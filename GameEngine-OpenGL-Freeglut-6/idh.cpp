#include "idh.h"
#include "r_window.h"

bool Input::is_key_down(char Key) {
    if (GetAsyncKeyState(Key) & 0x8000) {
        return true;
    }
    return false;
}

bool Input::is_key_down(int Key) {
    if (GetAsyncKeyState(Key) & 0x8000) {
        return true;
    }
    return false;
}

Input::Input(double& _DeltaTime, r_window* rw) : DeltaTime(_DeltaTime), rw(rw) {
    KEYs = util::array<KeyCodeData>();
    IAxis = util::array<InputAxis>();


    // in the future i should probably try getting all the buttons of the mouse 
    MouseButtonsArray = new KeyCodeData[AmountOfMouseButton];
}

Vec2 Input::get_mouse_position() {
    Vec2 WindowCenterPosition = rw->get_window_size();

    if (WindowCenterPosition.x == 0 || WindowCenterPosition.y == 0) {
        return Vec2(0, 0);
    }
    WindowCenterPosition = WindowCenterPosition / 2;
    Vec2 MousePosition = get_screen_passive_mouse_position() - WindowCenterPosition;

    return Vec2(MousePosition.x / WindowCenterPosition.x, (MousePosition.y / WindowCenterPosition.y) * -1);
}

Vec2 Input::get_angle_from_mouse_position_to_angle() {
    Vec2 WindowSize = rw->get_window_size();
    Vec2 RelativePosition = get_mouse_position();
    RelativePosition.y *= -1;
    float DirectionAngle = atan2(RelativePosition.y, RelativePosition.x) * (180.0f / extra_math_h::pi);
    float Distance = sqrt(RelativePosition.x * RelativePosition.x + RelativePosition.y * RelativePosition.y);
    float HorizontalAngle = RelativePosition.x * (rw->get_fov() * 0.5f);
    float VerticalAngle = RelativePosition.y * (rw->get_fov() * 0.5f * (WindowSize.y / WindowSize.x));
    return Vec2(HorizontalAngle, VerticalAngle);
}

Vec2 Input::get_screen_mouse_position() {
    return MouseDetails.Position;
}

Vec2 Input::get_screen_motion_mouse_position() {
    return MouseDetails.MotionPosition;
}

Vec2 Input::get_screen_passive_mouse_position() {
    return MouseDetails.PassiveMotionPosition;
}

bool Input::get_mouse_button(int MouseButton) {
    auto it = MouseButtonStates.find(MouseButton);
    if (it != MouseButtonStates.end()) {
        return it->second;
    }
    return false;
}

bool Input::was_mouse_button_just_pressed(int MouseButton) {
    for (size_t i = 0; i < sizeof(MouseButtonsArray) / sizeof(MouseButtonsArray[0]); i++) {
        if (MouseButtonsArray[i].LastState == false && MouseButtonsArray[i].CurrentState == true) {
            MouseDetails.ClickPosition = MouseDetails.Position;
            return true;
        }
    }
    return false;
}

bool Input::was_mouse_button_just_released(int MouseButton) {
    for (size_t i = 0; i < sizeof(MouseButtonsArray) / sizeof(MouseButtonsArray[0]); i++) {
        if (MouseButtonsArray[i].LastState == true && MouseButtonsArray[i].CurrentState == false) {
            MouseDetails.ClickPosition = Vec2::Zero();
            return true;
        }
    }
    return false;
}

Vec2 Input::get_on_click_location() {
    return MouseDetails.ClickPosition;
}

void Input::set_mouse_click(int Button, int State, Vec2 Position) {
    MouseDetails.Position = Position;
    bool isPressed = (State == GLUT_DOWN);

    // Store the click start position when button is first pressed
    if (isPressed && !get_mouse_button(Button)) {
        MouseClickStartPositions[Button] = Position;
    }

    MouseButtonStates[Button] = isPressed;
}

void Input::set_motion_mouse_position(Vec2 Position) {
    MouseDetails.MotionPosition = Position;
}

void Input::set_passive_mouse_position(Vec2 Position) {
    MouseDetails.PassiveMotionPosition = Position;

    if (_FreezeMouseToCenter) {
        Vec2 WindowPos = rw->get_window_position();
        Vec2 WindowSize = rw->get_window_size();
        int CenterX = (int)(WindowPos.x + WindowSize.x / 2);
        int CenterY = (int)(WindowPos.y + WindowSize.y / 2);

        if (abs(Position.x - CenterX) > 1 || abs(Position.y - CenterY) > 1) {
            SetCursorPos(CenterX, CenterY);
        }
    }
}

void Input::show_mouse(bool value) {
    if (value) {
        while (ShowCursor(TRUE) < 0) {}
    }
    else {
        while (ShowCursor(FALSE) >= 0) {}
    }
}

void Input::freeze_mouse_to_center(bool value) {
    _FreezeMouseToCenter = value;
}

void Input::update() {
    // Update previous mouse button states
    PreviousMouseButtonStates = MouseButtonStates;

    for (size_t i = 0; i < sizeof(MouseButtonsArray) / sizeof(MouseButtonsArray[0]); i++) {
        auto MouseButton = MouseButtonsArray[i];
        MouseButton.LastState = MouseButton.CurrentState;
        MouseButton.CurrentState = get_mouse_button(i);
        MouseButtonsArray[i] = MouseButton;
    }


    // Update key states
    for (int i = 0; i < KEYs.size(); i++) {
        auto keyData = KEYs[i];
        keyData.LastState = keyData.CurrentState;
        keyData.CurrentState = is_key_down(keyData.KeyCode);
        KEYs[i] = keyData;
    }
}

bool Input::was_key_just_released(char Key) {
    for (int i = 0; i < KEYs.size(); i++) {
        if (Key == KEYs[i].KeyCode && KEYs[i].LastState == true && KEYs[i].CurrentState == false) {
            return true;
        }
    }
    return false;
}

bool Input::was_key_just_pressed(char Key) {
    for (int i = 0; i < KEYs.size(); i++) {
        if (Key == KEYs[i].KeyCode && KEYs[i].LastState == false && KEYs[i].CurrentState == true) {
            return true;
        }
    }
    return false;
}

void Input::add_key(char Key) {
    KeyCodeData KD = KeyCodeData();
    KD.set_key(Key);
    KEYs.append(KD);
}

void Input::add_IAxis(InputAxis iaxis) {
    IAxis.append(iaxis);
}

float Input::get_IAxis(string axisName) {
    for (int i = 0; i < IAxis.size(); ++i) {
        InputAxis& axis = IAxis.get_ref(i);
        if (axis.AxisName == axisName) {
            bool pos = false, neg = false;
            for (int j = 0; j < axis.BIKeys.size(); ++j) {
                const auto& bk = axis.BIKeys.get_ref(j);
                if (is_key_down(bk.a))      pos = true;
                else if (is_key_down(bk.b)) neg = true;
            }
            if (pos)      axis.value += DeltaTime;
            else if (neg) axis.value -= DeltaTime;
            else if (axis.value != 0.0f) {
                float s = (axis.value > 0.0f ? 1.0f : -1.0f);
                axis.value -= s * DeltaTime;
                if (std::abs(axis.value) < 0.009f) axis.value = 0.0f;
            }

            axis.value = extra_math_h::clamp(axis.value, -1.0f, 1.0f);
            return axis.value;
        }
    }
    return 0.0f;
}