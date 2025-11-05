#include "r_window.h"
#include "callbacks.h"
#include "util.h"


util::static_mesh loaded_mesh;

r_window::r_window(int width, int height, const char* title) {
    this->width = width;
    this->height = height;
    this->title = title;
}

void r_window::init(int argc, char* argv[]) {
	camera = Camera();
    camera.eye = Vec3(0, 0, 5); // Camera position
    camera.center = Vec3(0, 0, 0);  // Point the camera is looking at
    camera.up = Vec3(0, 1, 0);

	loaded_mesh = util::load_obj("assets/bunny.obj");

	input = new Input(deltaTime, this);

	Input::InputAxis VerticalAxis("Vertical");
	VerticalAxis.add_key('W', 'S');
	VerticalAxis.add_key(VK_UP, VK_DOWN);

	Input::InputAxis HorizontalAxis("Horizontal");
	HorizontalAxis.add_key('A', 'D');
	HorizontalAxis.add_key(VK_LEFT, VK_RIGHT);

	Input::InputAxis RotateAxis("Rotate");
	RotateAxis.add_key('R', 'T');


	input->add_IAxis(VerticalAxis);
	input->add_IAxis(HorizontalAxis);
	input->add_IAxis(RotateAxis);

	callbacks::init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(width, height);
	glutCreateWindow(title);


	glutDisplayFunc(callbacks::display);
	glutMouseFunc(callbacks::mouse_click);
	glutMotionFunc(callbacks::mouse_motion);
	glutPassiveMotionFunc(callbacks::mouse_passive_motion);
	glutTimerFunc(frame_time, callbacks::timer, frame_time);

	// Switching camera to projection mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);
	gluPerspective(fov, width / height, 1, 1800);
	glMatrixMode(GL_MODELVIEW);


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
}

void r_window::clean_up() {
}

void r_window::timer() {
	currentFrame = (float)glutGet(GLUT_ELAPSED_TIME);              // seconds
	systemDeltaTime = (currentFrame - lastFrame) / 1000.0f;
	if (systemDeltaTime <= 0.0) systemDeltaTime = 1e-6;
	deltaTime = timeScale * systemDeltaTime;
	lastFrame = currentFrame;
	fps = static_cast<int>(1.0 / systemDeltaTime);

	//-------Debug-Info-------//
	LOG_DEBUG_R("FPS: " + to_string(fps) + " | Delta Time: " + to_string(deltaTime) + " | System Delta Time: " + to_string(systemDeltaTime));
	//-------End-------//

	input->update();

	glLoadIdentity();

	gluLookAt(
		camera.eye.x,
		camera.eye.y,
		camera.eye.z,
		camera.center.x,
		camera.center.y,
		camera.center.z,
		camera.up.x,
		camera.up.y,
		camera.up.z
	);



	if (input->get_IAxis("Vertical")) {
		double moveAmount = input->get_IAxis("Vertical") * deltaTime * 5.0f;
		camera.eye.z -= static_cast<float>(moveAmount);
		camera.center.z -= static_cast<float>(moveAmount);
	}

	if (input->get_IAxis("Horizontal")) {
		double moveAmount = input->get_IAxis("Horizontal") * deltaTime * 5.0f;
		camera.eye.x += static_cast<float>(moveAmount);
		camera.center.x += static_cast<float>(moveAmount);
	}

	if (input->get_IAxis("Rotate")) {
		double rotateAmount = input->get_IAxis("Rotate") * deltaTime * 50.0f;
		WorldRotaion.y += static_cast<float>(rotateAmount);
	}


	LOG_DEBUG("\nMouse passive position:" + input->get_screen_passive_mouse_position().tostr());

	glutPostRedisplay();
}

void r_window::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(WorldPostion.x, WorldPostion.y, WorldPostion.z);
	glRotatef(WorldRotaion.x, 1, 0, 0);
	glRotatef(WorldRotaion.y, 0, 1, 0);
	glRotatef(WorldRotaion.z, 0, 0, 1);


	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(Vec3), loaded_mesh.vertices);
		glPushMatrix();
			glDrawElements(GL_TRIANGLES, loaded_mesh.v_vertex_indices_count, GL_UNSIGNED_INT, loaded_mesh.v_vertex_indices);
		glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);



	glPopMatrix();
	glutSwapBuffers();
}

void r_window::mouse_click(int button, int state, int x, int y) {
	input->set_mouse_click(button, state, Vec2(static_cast<float>(x), static_cast<float>(y)));
}

void r_window::mouse_motion(int x, int y) {
	input->set_motion_mouse_position(Vec2(static_cast<float>(x), static_cast<float>(y)));
}

void r_window::mouse_passive_motion(int x, int y) {
	input->set_passive_mouse_position(Vec2(static_cast<float>(x), static_cast<float>(y)));
}