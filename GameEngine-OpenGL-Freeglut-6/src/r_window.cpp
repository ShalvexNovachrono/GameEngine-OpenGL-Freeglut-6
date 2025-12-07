#include "../include/r_window.h"
#include "../include/callbacks.h"
#include "../include/util.h"
#include "../include/world_container.h"
#include "../include/component/Camera.h"


util::static_mesh loaded_mesh;

r_window::r_window(const int& width, const int&  height, const string& title) {
    this->width = width;
    this->height = height;
    this->title = title;
}

void r_window::init(int argc, char* argv[]) {
	#pragma region Setup Input
	input = new Input(deltaTime, this);

	Input::InputAxis VerticalAxis("Vertical");
	VerticalAxis.addKey('W', 'S');
	VerticalAxis.addKey(VK_UP, VK_DOWN);

	Input::InputAxis HorizontalAxis("Horizontal");
	HorizontalAxis.addKey('A', 'D');
	HorizontalAxis.addKey(VK_LEFT, VK_RIGHT);

	Input::InputAxis RotateAxis("Rotate");
	RotateAxis.addKey('R', 'T');

	input->addIAxis(VerticalAxis);
	input->addIAxis(HorizontalAxis);
	input->addIAxis(RotateAxis);


	input->addKey(VK_ESCAPE);
	input->addKey('L');
	#pragma endregion

	#pragma region Setup World
	world = new world_container(this);
	#pragma endregion 


	callbacks::init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(width, height);
	glutCreateWindow(title.c_str());

	glutDisplayFunc(callbacks::display);
	glutReshapeFunc(callbacks::reshapeWindow);
	glutMouseFunc(callbacks::mouseClick);
	glutMotionFunc(callbacks::mouseMotion);
	glutPassiveMotionFunc(callbacks::mousePassiveMotion);
	glutMouseWheelFunc(callbacks::mouseScrollwheel);
	glutTimerFunc(frameTime, callbacks::timer, frameTime);

	// Switching camera to projection mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);
	gluPerspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.1, 1800);
	glMatrixMode(GL_MODELVIEW);


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();

}

void r_window::cleanUp() {
	delete input;
}

void r_window::start() {
	didTimerGetCalled = true;
	//util::mesh_holder_instance = util::mesh_holder();
	//util::textures_holder_instance = util::textures_holder();

	input->addKey('V');
	input->addKey('N');
	
	util::mesh_holder_instance.loadStaticMeshToHolder("Stanford Bunny", "assets/bunny.obj");
	util::mesh_holder_instance.loadStaticMeshToHolder("Ground", "assets/Flat-Ground-Grass.obj");

	loaded_mesh = util::mesh_holder_instance.getStaticMesh("Stanford Bunny");
	
	world->init();
	
	//textures.push_back(make_unique<util::texture_data>());
	//textures.back()->loadTexture("assets/Test_grid_2000x2000.png");
}

void r_window::timer() {
	currentFrame = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));              // seconds
	systemDeltaTime = (currentFrame - lastFrame) / 1000.0f;
	if (systemDeltaTime <= 0.0f) systemDeltaTime = 1e-6f;
	deltaTime = timeScale * systemDeltaTime;
	lastFrame = currentFrame;
	fps = static_cast<int>(1.0f / systemDeltaTime);

	//-------Debug-Info-------//
	LOG_DEBUG_R("FPS: " + to_string(fps) + " | Delta Time: " + to_string(deltaTime) + " | System Delta Time: " + to_string(systemDeltaTime));
	//-------End-------//

	input->update();
	glLoadIdentity();
	
	if (!didTimerGetCalled) start();

	if (input->wasKeyJustPressed('V')) {
		loaded_mesh = util::mesh_holder_instance.getStaticMesh("Ground");
	}

	
	if (input->wasKeyJustPressed('N')) {
		loaded_mesh = util::mesh_holder_instance.getStaticMesh("Stanford Bunny");
	}
	
	world->update();




	glutPostRedisplay();
}

void r_window::draw() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(worldPostion.x, worldPostion.y, worldPostion.z);
	glRotatef(worldRotaion.x, 1, 0, 0);
	glRotatef(worldRotaion.y, 0, 1, 0);
	glRotatef(worldRotaion.z, 0, 0, 1);


	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(Vec3), loaded_mesh.vertices);
		glPushMatrix();
			glDrawElements(GL_TRIANGLES, loaded_mesh.v_vertex_indices_count, GL_UNSIGNED_INT, loaded_mesh.v_vertex_indices);
		glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);



	glPopMatrix();
	glutSwapBuffers();
}

void r_window::mouseClick(int button, int state, int x, int y) {
	input->setMouseClick(button, state, Vec2(static_cast<float>(x), static_cast<float>(y)));
}

void r_window::mouseMotion(int x, int y) {
	input->setMotionMousePosition(Vec2(static_cast<float>(x), static_cast<float>(y)));
}

void r_window::mousePassiveMotion(int x, int y) {
	input->setPassiveMousePosition(Vec2(static_cast<float>(x), static_cast<float>(y)));
}

void r_window::mouseScrollwheel(int button, int scrollDirection, int x, int y) {
	input->setMouseScrollwheelValues(button, scrollDirection, Vec2(static_cast<float>(x), static_cast<float>(y)));
}

void r_window::reshapeWindow(int width, int height) {
	this->width = width;
	this->height = height;
	glutReshapeWindow(width, height);
	updateWindowDetails();
}

void r_window::setFov(float new_fov) {
	if (new_fov < minimumFov) return;
	if (new_fov > maximumFov) return;
	fov = new_fov;
	updateWindowDetails();
}

void r_window::updateWindowDetails() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(fov, width / height, 0.1, 1800);
	glMatrixMode(GL_MODELVIEW);
}

Input& r_window::getInputRef() {
	return *input;
}
