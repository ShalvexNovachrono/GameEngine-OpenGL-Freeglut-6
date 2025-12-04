#include "../include/camera.h"
#include "../include/r_window.h"

Camera::Camera(r_window* rw) : rw(rw), input(rw->getInputRef()) {
    eye = Vec3(0, 0, 3); // Camera position
    up = Vec3(0, 1, 0);
    front = Vec3(0, 0, -1);

    input.addKey('W');
    input.addKey('S');
    input.addKey('A');
    input.addKey('D');
    input.addKey(VK_SHIFT);
    input.addKey('F');
    input.addKey('H');
}

void Camera::update() {
    if (input.wasKeyJustPressed('F')) {
        freeze_toggle_mouse = !freeze_toggle_mouse;
        input.freezeMouseToCenter(freeze_toggle_mouse);
    }

    if (input.wasKeyJustPressed('H')) {
        hide_toggle_mouse = !hide_toggle_mouse; 
        input.showMouse(hide_toggle_mouse);
    }


#pragma region Movement
    float speed = movement_speed;

    if (input.isKeyDown(VK_SHIFT)) {
        speed += 5;
    }

    if (input.isKeyDown('W')) eye += front * speed * rw->getDeltaTime();

    if (input.isKeyDown('S')) eye -= front * speed * rw->getDeltaTime();

    Vec3 right = Vec3::Cross(front, up).Normalize();

    if (input.isKeyDown('A')) eye -= right * speed * rw->getDeltaTime();

    if (input.isKeyDown('D')) eye += right * speed * rw->getDeltaTime();

#pragma endregion

#pragma region Rotation
    Vec2 mouse_delta_position = input.getMouseDelta();
    if (input.getMouseButton(0)) {

        float x_offset = mouse_delta_position.x * rotation_sensitivity;
        float y_offset = -mouse_delta_position.y * rotation_sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        if (yaw > 360.0f) yaw -= 360.0f;
        else if (yaw < -360.0f) yaw += 360.0f;

        pitch = clamp(pitch, -89.0f, 89.0f);

        float yaw_r = degrees_to_radians(yaw);
        float pitch_r = degrees_to_radians(pitch);

        Vec3 direction = Vec3::Zero();
        direction.x = cosf(yaw_r) * cosf(pitch_r);
        direction.y = sinf(pitch_r);
        direction.z = sinf(yaw_r) * cosf(pitch_r);

        front = direction.Normalised();
    }

    int scrollDirection = input.getMouseScroll();

    if (scrollDirection != 0) {
        float current_fov = rw->getFov();
        float new_fov = current_fov + (scrollDirection * -2.0f);
        rw->setFov(new_fov);
    }

#pragma endregion
}
