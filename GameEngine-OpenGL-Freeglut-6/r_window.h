#ifndef r_window_h
#define r_window_h

#include "main.h"
#include "extra_math.h"
#include "GameObject.h"
#include "camera.h"

class r_window {
public:
	Camera* camera;

	r_window(int width, int height, string title);
	~r_window() = default;

	void init(int argc, char* argv[]);

	void clean_up();

	void timer();

	void draw();

	void mouse_click(int button, int state, int x, int y);

	void mouse_motion(int x, int y);

	void mouse_passive_motion(int x, int y);

	float get_delta_time() const { return delta_time; }

	float get_system_delta_time() const { return system_delta_time; }

	float get_time_scale() const { return time_scale; }

	void set_time_scale(float scale) { time_scale = scale; }

	int get_fps() const { return fps; }

	float get_fov() const { return fov; }

	Vec2 get_window_size() const { return Vec2(static_cast<float>(width), static_cast<float>(height)); }

	Vec2 get_window_position() const { return Vec2(static_cast<float>(glutGet(GLUT_WINDOW_X)), static_cast<float>(glutGet(GLUT_WINDOW_Y))); }

	Input& get_input_reference();
private:
    int width = 100, height = 100;
    string title;

    float delta_time = 0.0f;
	float system_delta_time = 0.0f;
	float last_frame = 0.0f;
	float time_scale = 1.0f;
	float current_frame = 0.0f;
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
