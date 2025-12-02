#ifndef input_h
#define input_h

#include <windows.h>
#include <WinUser.h>
#include <string>
#include <unordered_map>
#include "util.h"

using namespace std;

class r_window;

class Input {
public:
    // private is moved to the bottom

    struct MouseScrollWheel {
        int button;
        int scrollDirection;
        Vec2 position;
    };


    //--------MouseDetails--------//
    struct MouseDetail {
        Vec2 position;
        Vec2 motionPosition;
        Vec2 passiveMotionPosition;
        MouseScrollWheel scrollWheel;

        /// <summary>
        /// This will be used for when mouse is dragging
        /// </summary>
        Vec2 clickPosition;
    };



    //--------KeyCodeData--------//
    class KeyCodeData {
    public:
        bool lastState;
        bool currentState;
        int keyCode = 0;
        KeyCodeData() {
            lastState = false;
            currentState = false;
        }

        void set_key(int Key) {
            keyCode = Key;
        }
    };

    util::array<KeyCodeData> KEYs;

    //--------Input-Axis--------//
    class BIPolarKeys {
    public:
        int a, b;

        BIPolarKeys(int a = 0, int b = 0) : a(a), b(b) {};

    };

    class InputAxis {
    public:
        string axisName;
        float value = 0;
        util::array<BIPolarKeys> BIKeys;
        InputAxis(string axisName = "") {
            this->axisName = axisName;
        }

        void addKey(int Key1, int Key2) {
            BIKeys.append(BIPolarKeys(Key1, Key2));
        }
    };


    util::array<InputAxis> IAxis;


    //--------Constructor--------//
    Input(float& _DeltaTime, r_window* rw);

    /// <summary>
    /// Returns the mouse position
    /// </summary>
    /// <returns>Vec2 coordinates.</returns>
    Vec2 getMousePosition();

    /// <summary>
    /// Returns the mouse position in screen coordinates (0 to window size).
    /// This value updates when a mouse click event occurs.
    /// </summary>
    /// <returns>Vec2 in pixel coordinates.</returns>
    Vec2 getScreenMousePosition();

    /// <summary>
    /// Returns the mouse position in screen coordinates (0 to window size).
    /// This value updates continuously while the mouse is being dragged (button held down).
    /// </summary>
    /// <returns>Vec2 in pixel coordinates.</returns>
    Vec2 getScreenMotionMousePosition();

    /// <summary>
    /// Returns the mouse position in screen coordinates (0 to window size).
    /// This value updates with any mouse movement when no buttons are pressed (passive motion).
    /// </summary>
    /// <returns>Vec2 in pixel coordinates.</returns>
    Vec2 getScreenPassiveMousePosition();

    /// <summary>
    /// Returns the mouse delta
    /// Difference between current mouse position and last mouse position
    /// </summary>
    /// <returns></returns>
    Vec2 getMouseDelta();

    /// <summary>
    /// Returns the scroll direction 
    /// </summary>
    /// <returns></returns>
    int getMouseScroll();

    /// <summary>
    /// Checks if [MouseButton] is pressed and RETURNS true or false
    /// </summary>
    /// <param name="MouseButton"></param>
    /// <returns>bool</returns>
    bool getMouseButton(int MouseButton);

    void setMouseClick(int Button, int State, Vec2 position);

    void setMotionMousePosition(Vec2 position);

    void setPassiveMousePosition(Vec2 position);

    void setMouseScrollwheelValues(int button, int scrollDirection, Vec2 position);

    void showMouse(bool value);

    void freezeMouseToCenter(bool value);

    /// <summary>
    /// Returns true only on the frame when the specified mouse button is pressed.
    /// </summary>
    /// <param name="MouseButton">The mouse button to check (0 = left, 1 = middle, 2 = right)</param>
    /// <returns>True if the button was just pressed this frame</returns>
    bool wasMouseButtonJustPressed(int MouseButton);

    /// <summary>
    /// Returns true only on the frame when the specified mouse button is released.
    /// </summary>
    /// <param name="MouseButton">The mouse button to check (0 = left, 1 = middle, 2 = right)</param>
    /// <returns>True if the button was just released this frame</returns>
    bool wasMouseButtonJustReleased(int MouseButton);

    /// <summary>
    /// This will return where mouse was clicked
    /// </summary>
    /// <returns></returns>
    Vec2 getOnClickLocation();

    /// <summary>
    /// Checks if [Key] is pressed and RETURNS true or false
    /// </summary>
    /// <param name="key">char</param>
    /// <returns>bool</returns>
    bool isKeyDown(char Key);

    /// <summary>
    /// Checks if [Key] is pressed and RETURNS true or false
    /// </summary>
    /// <param name="key">int</param>
    /// <returns>bool</returns>
    bool isKeyDown(int Key);

    /// <summary>
    /// Updates the state of all tracked keys.
    /// Should be called once per frame to correctly detect key presses and releases.
    /// </summary>
    void update();

    /// <summary>
    /// Returns true only on the frame when the specified key is released (i.e., transitions from pressed to not pressed).
    /// Use this to detect single key release events.
    /// </summary>
    /// <param name="Key">The key to check.</param>
    /// <returns>True if the key was just released this frame; otherwise, false.</returns>
    bool wasKeyJustReleased(int Key);

    /// <summary>
    /// Returns true only on the frame when the specified key is pressed (i.e., transitions from not pressed to pressed).
    /// Use this to detect single key press events.
    /// </summary>
    /// <param name="Key">The key to check.</param>
    /// <returns>True if the key was just pressed this frame; otherwise, false.</returns>
    bool wasKeyJustPressed(int Key);

    /// <summary>
    /// Adds a key to the list of tracked keys for input state monitoring.
    /// Call this before using WasKeyJustPressed or WasKeyJustReleased for a key.
    /// </summary>
    /// <param name="Key">The key to start tracking.</param>
    void addKey(int Key);

    /// <summary>
    /// This function will add the iaxis to a dynamic_array
    /// </summary>
    /// <param name="iaxis"></param>
    void addIAxis(InputAxis iaxis);

    /// <summary>
    /// This function will look for the iaxis that has the name and get axis value
    /// based on what keys are pressed
    /// </summary>
    /// <param name="AxisName"></param>
    /// <returns></returns>
    float getIAxis(string axisName);
private:
    //--------Mouse-Info--------//
    MouseDetail mouseDetails;
    Vec2 motionDirection;
    Vec2 lastMousePosition = Vec2::Zero();
    unordered_map<int, bool> mouseButtonStates;
    unordered_map<int, bool> previousMouseButtonStates;
    unordered_map<int, Vec2> mouseClickStartPositions;
    bool _freezeMouseToCenter = false;
    size_t amountOfMouseButton = 3; // the main three buttons (left, right and middle mouse button)
    KeyCodeData* mouseButtonsArray = new KeyCodeData[3];

    //--------DeltaTime&Window--------//
    float& deltaTime;
    r_window* rw;
};

#endif // input_h