#pragma once
#include "base_component.h"
#include "../extra_math.h"
#include "../GameObject.h"

class r_window;

class Camera : public base_component {
	r_window* rw = nullptr;
	float yaw = -90, pitch = 0;

	bool freeze_toggle_mouse = false;
	bool hide_toggle_mouse = false;

public:
	Vec3 eye, up, front;
	float movement_speed = 10.0f;
	float rotation_sensitivity = 0.1f;
	bool first_mouse_focus = true;

	Camera();
	void start() override;
	void update() override;
	void setGameObject(GameObject* gm, const int& id) override;
	Vec3 center() const;
	Camera* clone() override;
};
