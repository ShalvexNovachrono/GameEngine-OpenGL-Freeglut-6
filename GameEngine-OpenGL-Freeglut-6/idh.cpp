#include "idh.h"
#include "r_window.h"

bool Input::isKeyDown(char Key) {
    if (GetAsyncKeyState(Key) & 0x8000) {
        return true;
    }
    return false;
}

bool Input::isKeyDown(int Key) {
    if (GetAsyncKeyState(Key) & 0x8000) {
        return true;
    }
    return false;
}

Input::Input(float& _DeltaTime, r_window* rw) : deltaTime(_DeltaTime), rw(rw) {
    KEYs = util::array<KeyCodeData>();
    IAxis = util::array<InputAxis>();

    lastMousePosition = getMousePosition();

    // in the future i should probably try getting all the buttons of the mouse 
    mouseButtonsArray = new KeyCodeData[amountOfMouseButton];
}

Vec2 Input::getMousePosition() {
    bool is_pressing = false;

    for (size_t i = 0; i < amountOfMouseButton; i++) {
        if (mouseButtonsArray[i].currentState == true) {
            is_pressing = true;
            break;
        }
    }

    return (is_pressing) ? mouseDetails.motionPosition : mouseDetails.passiveMotionPosition;
}

Vec2 Input::getScreenMousePosition() {
    return mouseDetails.position;
}

Vec2 Input::getScreenMotionMousePosition() {
    return mouseDetails.motionPosition;
}

Vec2 Input::getScreenPassiveMousePosition() {
    return mouseDetails.passiveMotionPosition;
}

Vec2 Input::getMouseDelta() {
    Vec2 CurrentPosition = getMousePosition();

    Vec2 Difference = CurrentPosition - lastMousePosition;

    lastMousePosition = CurrentPosition;

    return Difference;
}

int Input::getMouseScroll() {
    int direction = mouseDetails.scrollWheel.scrollDirection;

    mouseDetails.scrollWheel.scrollDirection = 0;

    return direction;
}

bool Input::getMouseButton(int MouseButton) {
    auto it = mouseButtonStates.find(MouseButton);
    if (it != mouseButtonStates.end()) {
        return it->second;
    }
    return false;
}

bool Input::wasMouseButtonJustPressed(int MouseButton) {
    if (MouseButton < 0 || MouseButton >= static_cast<int>(amountOfMouseButton)) {
        return false;
    }

    if (mouseButtonsArray[MouseButton].lastState == false && mouseButtonsArray[MouseButton].currentState == true) {
        mouseDetails.clickPosition = mouseDetails.position;
        return true;
    }
    
    return false;
}

bool Input::wasMouseButtonJustReleased(int MouseButton) {
    if (MouseButton < 0 || MouseButton >= static_cast<int>(amountOfMouseButton)) {
        return false;
    }

    if (mouseButtonsArray[MouseButton].lastState == true && mouseButtonsArray[MouseButton].currentState == false) {
        mouseDetails.clickPosition = Vec2::Zero();
        return true;
    }
    
    return false;
}

Vec2 Input::getOnClickLocation() {
    return mouseDetails.clickPosition;
}

void Input::setMouseClick(int Button, int State, Vec2 position) {
    mouseDetails.position = position;
    bool isPressed = (State == GLUT_DOWN);

    // Store the click start position when button is first pressed
    if (isPressed && !getMouseButton(Button)) {
        mouseClickStartPositions[Button] = position;
    }

    mouseButtonStates[Button] = isPressed;
}

void Input::setMotionMousePosition(Vec2 position) {

    if (_freezeMouseToCenter) {
        Vec2 WindowPos = rw->getWindowPosition();
        Vec2 WindowSize = rw->getWindowSize();
        int CenterX = (int)(WindowPos.x + WindowSize.x / 2);
        int CenterY = (int)(WindowPos.y + WindowSize.y / 2);

        if (abs(position.x - CenterX) > 1 || abs(position.y - CenterY) > 1) {
            SetCursorPos(CenterX, CenterY);
        }
    }
    mouseDetails.motionPosition = position;
}

void Input::setPassiveMousePosition(Vec2 position) {
    if (_freezeMouseToCenter) {
        Vec2 WindowPos = rw->getWindowPosition();
        Vec2 WindowSize = rw->getWindowSize();
        int CenterX = (int)(WindowPos.x + WindowSize.x / 2);
        int CenterY = (int)(WindowPos.y + WindowSize.y / 2);

        if (abs(position.x - CenterX) > 1 || abs(position.y - CenterY) > 1) {
            SetCursorPos(CenterX, CenterY);
        }
    }
    mouseDetails.passiveMotionPosition = position;

}

void Input::setMouseScrollwheelValues(int button, int scrollDirection, Vec2 position) {
    mouseDetails.scrollWheel.button = button;
    mouseDetails.scrollWheel.scrollDirection = scrollDirection;
    mouseDetails.scrollWheel.position = position;
}

void Input::showMouse(bool value) {
    if (value) {
        while (ShowCursor(TRUE) < 0) {}
    }
    else {
        while (ShowCursor(FALSE) >= 0) {}
    }
}

void Input::freezeMouseToCenter(bool value) {
    _freezeMouseToCenter = value;
}

void Input::update() {
    // Update previous mouse button states
    previousMouseButtonStates = mouseButtonStates;

    for (size_t i = 0; i < amountOfMouseButton; i++) {
        auto& MouseButton = mouseButtonsArray[i];
        MouseButton.lastState = MouseButton.currentState;
        MouseButton.currentState = getMouseButton(i);
        mouseButtonsArray[i] = MouseButton;
    }


    // Update key states
    for (int i = 0; i < KEYs.size(); i++) {
        auto& keyData = KEYs[i];
        keyData.lastState = keyData.currentState;
        keyData.currentState = isKeyDown(keyData.keyCode);
        KEYs[i] = keyData;
    }

   
}

bool Input::wasKeyJustReleased(int Key) {
    for (int i = 0; i < KEYs.size(); i++) {
        if (Key == KEYs[i].keyCode && KEYs[i].lastState == true && KEYs[i].currentState == false) {
            return true;
        }
    }
    return false;
}

bool Input::wasKeyJustPressed(int Key) {
    for (int i = 0; i < KEYs.size(); i++) {
        if (Key == KEYs[i].keyCode && KEYs[i].lastState == false && KEYs[i].currentState == true) {
            return true;
        }
    }
    return false;
}

void Input::addKey(int Key) {
    KeyCodeData KD = KeyCodeData();
    KD.set_key(Key);
    KEYs.append(KD);
}

void Input::addIAxis(InputAxis iaxis) {
    IAxis.append(iaxis);
}

float Input::getIAxis(string axisName) {
    for (int i = 0; i < IAxis.size(); ++i) {
        InputAxis& axis = IAxis.get_ref(i);
        if (axis.axisName == axisName) {
            bool pos = false, neg = false;
            for (int j = 0; j < axis.BIKeys.size(); ++j) {
                const auto& bk = axis.BIKeys.get_ref(j);
                if (isKeyDown(bk.a))      pos = true;
                else if (isKeyDown(bk.b)) neg = true;
            }
            if (pos)      axis.value += deltaTime;
            else if (neg) axis.value -= deltaTime;
            else if (axis.value != 0.0f) {
                float s = (axis.value > 0.0f ? 1.0f : -1.0f);
                axis.value -= s * deltaTime;
                if (std::abs(axis.value) < 0.009f) axis.value = 0.0f;
            }

            axis.value = extra_math_h::clamp(axis.value, -1.0f, 1.0f);
            return axis.value;
        }
    }
    return 0.0f;
}