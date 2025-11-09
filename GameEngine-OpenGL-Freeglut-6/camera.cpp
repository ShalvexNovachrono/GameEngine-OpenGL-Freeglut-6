#include "camera.h"
#include "r_window.h"

Camera::Camera(r_window* rw) : rw(rw), input(rw->get_input_reference()) {
    eye = Vec3(0, 0, 3); // Camera position
    up = Vec3(0, 1, 0);
    front = Vec3(0, 0, -1);

    last_x_position = rw->get_window_size().x / 2;
    last_y_position = rw->get_window_size().y / 2;

    input.add_key('W');
    input.add_key('S');
    input.add_key('A');
    input.add_key('D');
    input.add_key(VK_SHIFT);
    input.add_key('F');
    input.add_key('H');
}

void Camera::update() {
    if (input.was_key_just_pressed('F')) {
        input.freeze_mouse_to_center(freeze_toggle_mouse);
        freeze_toggle_mouse = !freeze_toggle_mouse;
    }

    if (input.was_key_just_pressed('H')) {
        hide_toggle_mouse = !hide_toggle_mouse;
        input.show_mouse(hide_toggle_mouse);
    }


#pragma region Movement
    float speed = movement_speed;

    if (input.is_key_down(VK_SHIFT)) {
        speed += 5;
    }

    if (input.is_key_down('W')) {
        eye += front * speed * rw->get_delta_time();
    }

    if (input.is_key_down('S')) {
        eye -= front * speed * rw->get_delta_time();
    }

    if (input.is_key_down('A')) {
        eye -= Vec3::Cross(front, up).Normalize() * speed * rw->get_delta_time();
    }

    if (input.is_key_down('D')) {
        eye += Vec3::Cross(front, up).Normalize() * speed * rw->get_delta_time();
    }

#pragma endregion

#pragma region Rotation
    
    if (input.get_mouse_button(2)) {
        Vec2 mouse_position = input.get_screen_motion_mouse_position();
        if (first_mouse_focus) {
            last_x_position = mouse_position.x;
            last_y_position = mouse_position.y;
            first_mouse_focus = false;
        }


        float x_offset = mouse_position.x - last_x_position;
        float y_offset = last_y_position - mouse_position.y;

        x_offset *= rotation_sensitivity;
        y_offset *= rotation_sensitivity;

        yaw   += x_offset;
        pitch += y_offset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        Vec3 direction = Vec3::Zero();

        direction.x = cos(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch));
        direction.y = sin(degrees_to_radians(pitch));
        direction.z = sin(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch));

        LOG_DEBUG_R("\n" + direction.tostr() );
        front = direction.Normalize();
    }

#pragma endregion
}
