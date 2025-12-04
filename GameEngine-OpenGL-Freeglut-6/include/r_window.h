#ifndef r_window_h
#define r_window_h

#include "main.h"
#include "extra_math.h"
#include "idh.h"
#include "camera.h"

class r_window {
public:
	Camera* camera;

	r_window(const int& width, const int&  height, const string& title);
	~r_window() = default;

	void init(int argc, char* argv[]);

	void cleanUp();

	void start();

	void timer();

	void draw() const;

	void mouseClick(int button, int state, int x, int y);

	void mouseMotion(int x, int y);

	void mousePassiveMotion(int x, int y);

	void mouseScrollwheel(int button, int scrollDirection, int x, int y);

	float getDeltaTime() const { return deltaTime; }

	float getSystemDeltaTime() const { return systemDeltaTime; }

	float getTimeScale() const { return timeScale; }

	void setTimeScale(float scale) { timeScale = scale; }

	void reshapeWindow(int width, int height);

	int getFps() const { return fps; }

	float getFov() const { return fov; }

	void setFov(float new_fov);

	void updateWindowDetails();

	Vec2 getWindowSize() const { return { static_cast<float>(width), static_cast<float>(height) }; }

	Vec2 getWindowPosition() const { return { static_cast<float>(glutGet(GLUT_WINDOW_X)), static_cast<float>(glutGet(GLUT_WINDOW_Y))}; }

	Input& getInputRef();
private:
    int width = 100, height = 100;
    string title;

    float deltaTime = 0.0f;
	float systemDeltaTime = 0.0f;
	float lastFrame = 0.0f;
	float timeScale = 1.0f;
	float currentFrame = 0.0f;
    int fps = 0;

	float fov = 45.0f;
	const float minimumFov = 1.0f;
	const float maximumFov = 110.0f;
	const float frameRate = 145;
	const int frameTime = static_cast<int>(1000.0f / frameRate);



	Input* input;
	//vector<unique_ptr<util::texture_data>> textures;

	Vec3 worldPostion = Vec3(0, 0, 0);
	Vec3 worldRotaion = Vec3(0, 0, 0);
    Vec4 clearColor = Vec4(0.2f, 0.3f, 0.3f, 1.0f);

	bool didTimerGetCalled = false;
};
#endif
