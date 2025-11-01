#ifndef r_window_h
#define r_window_h

#include "main.h"
#include "extra_math.h"
#include "GameObject.h"
#include "camera.h"

class r_window {
public:
	Camera camera;

	r_window() = default;
	r_window(int width, int height, const char* title);
	~r_window() = default;

	void init(int argc, char* argv[]);

	void clean_up();

	void timer();

	void draw();

	void mouse_click(int button, int state, int x, int y);

	void mouse_motion(int x, int y);

	void mouse_passive_motion(int x, int y);

	double get_delta_time() const { return deltaTime; }

	double get_system_delta_time() const { return systemDeltaTime; }

	double get_time_scale() const { return timeScale; }

	void set_time_scale(double scale) { timeScale = scale; }

	int get_fps() const { return fps; }

	float get_fov() const { return fov; }

	Vec2 get_window_size() const { return Vec2(static_cast<float>(width), static_cast<float>(height)); }

	Vec2 get_window_position() const { return Vec2(static_cast<float>(glutGet(GLUT_WINDOW_X)), static_cast<float>(glutGet(GLUT_WINDOW_Y))); }
private:
    int width = 800, height = 600;
    const char* title = "Shalvex Novachrono's OpenGL Window";

    double deltaTime = 0.0f;
    double systemDeltaTime = 0.0f;
    double lastFrame = 0.0f;
    double timeScale = 1.0f;
    double currentFrame = 0.0f;
    int fps = 0;

	const float fov = 45.0f;
	const float frame_rate = 165;
	const int frame_time = static_cast<int>(1000.0f / frame_rate);



	Input* input;


	Vec3 WorldPostion = Vec3(0, 0, 0);
	Vec3 WorldRotaion = Vec3(0, 0, 0);
    Vec4 clearColor = Vec4(0.2f, 0.3f, 0.3f, 1.0f);
};
#endif
