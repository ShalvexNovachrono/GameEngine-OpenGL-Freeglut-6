#ifndef camera_h
#define camera_h

#include "extra_math.h"

class r_window;
class Input;

class Camera {
	r_window* rw;
	Input& input;

	float yaw = -90, pitch = 0;

	bool freeze_toggle_mouse = false;
	bool hide_toggle_mouse = false;

public:
	Vec3 eye, up, front;
	float movement_speed = 1.0f;
	float rotation_sensitivity = 0.1f;
	bool first_mouse_focus = true;

	Camera(r_window* rw);
	~Camera() = default;

	void update();
	inline Vec3 center() const { return eye + front; }
};

#endif // !camera_h