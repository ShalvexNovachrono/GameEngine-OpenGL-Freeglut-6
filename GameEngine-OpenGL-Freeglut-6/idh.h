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


    //--------MouseDetails--------//
    struct MouseDetail {
        Vec2 Position;
        Vec2 MotionPosition;
        Vec2 PassiveMotionPosition;

        /// <summary>
        /// This will be used for when mouse is dragging
        /// </summary>
        Vec2 ClickPosition;
    };



    //--------KeyCodeData--------//
    class KeyCodeData {
    public:
        bool LastState;
        bool CurrentState;
        char KeyCode = 0;
        KeyCodeData() {
            LastState = false;
            CurrentState = false;
        }

        void set_key(char Key) {
            KeyCode = Key;
        }
    };

    util::array<KeyCodeData> KEYs;

    //--------Input-Axis--------//
    class BIPolarKeys {
    public:
        char a, b;

        BIPolarKeys(char a = 0, char b = 0) : a(a), b(b) {};

    };

    class InputAxis {
    public:
        std::string AxisName;
        float value = 0;
        util::array<BIPolarKeys> BIKeys;
        InputAxis(string AxisName = "") {
            this->AxisName = AxisName;
        }

        void add_key(char Key1, char Key2) {
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
    Vec2 get_mouse_position();

    /// <summary>
    /// Calculates the horizontal and vertical angular offsets from the center of
    /// the screen to the current mouse position.
    /// Uses the engine's field of view (FOV) and window aspect ratio for scaling.
    /// </summary>
    /// <returns>Vec2 containing horizontal and vertical angles in degrees.</returns>
    //Vec2 get_angle_from_mouse_position_to_angle();

    /// <summary>
    /// Returns the mouse position in screen coordinates (0 to window size).
    /// This value updates when a mouse click event occurs.
    /// </summary>
    /// <returns>Vec2 in pixel coordinates.</returns>
    Vec2 get_screen_mouse_position();

    /// <summary>
    /// Returns the mouse position in screen coordinates (0 to window size).
    /// This value updates continuously while the mouse is being dragged (button held down).
    /// </summary>
    /// <returns>Vec2 in pixel coordinates.</returns>
    Vec2 get_screen_motion_mouse_position();

    /// <summary>
    /// Returns the mouse position in screen coordinates (0 to window size).
    /// This value updates with any mouse movement when no buttons are pressed (passive motion).
    /// </summary>
    /// <returns>Vec2 in pixel coordinates.</returns>
    Vec2 get_screen_passive_mouse_position();

    /// <summary>
    /// Checks if [MouseButton] is pressed and RETURNS true or false
    /// </summary>
    /// <param name="MouseButton"></param>
    /// <returns>bool</returns>
    bool get_mouse_button(int MouseButton);

    void set_mouse_click(int Button, int State, Vec2 Position);

    void set_motion_mouse_position(Vec2 Position);

    void set_passive_mouse_position(Vec2 Position);

    void show_mouse(bool value);

    void freeze_mouse_to_center(bool value);

    /// <summary>
    /// Returns true only on the frame when the specified mouse button is pressed.
    /// </summary>
    /// <param name="MouseButton">The mouse button to check (0 = left, 1 = middle, 2 = right)</param>
    /// <returns>True if the button was just pressed this frame</returns>
    bool was_mouse_button_just_pressed(int MouseButton);

    /// <summary>
    /// Returns true only on the frame when the specified mouse button is released.
    /// </summary>
    /// <param name="MouseButton">The mouse button to check (0 = left, 1 = middle, 2 = right)</param>
    /// <returns>True if the button was just released this frame</returns>
    bool was_mouse_button_just_released(int MouseButton);

    /// <summary>
    /// This will return where mouse was clicked
    /// </summary>
    /// <returns></returns>
    Vec2 get_on_click_location();

    /// <summary>
    /// Checks if [Key] is pressed and RETURNS true or false
    /// </summary>
    /// <param name="key">char</param>
    /// <returns>bool</returns>
    bool is_key_down(char Key);

    /// <summary>
    /// Checks if [Key] is pressed and RETURNS true or false
    /// </summary>
    /// <param name="key">int</param>
    /// <returns>bool</returns>
    bool is_key_down(int Key);

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
    bool was_key_just_released(char Key);

    /// <summary>
    /// Returns true only on the frame when the specified key is pressed (i.e., transitions from not pressed to pressed).
    /// Use this to detect single key press events.
    /// </summary>
    /// <param name="Key">The key to check.</param>
    /// <returns>True if the key was just pressed this frame; otherwise, false.</returns>
    bool was_key_just_pressed(char Key);

    /// <summary>
    /// Adds a key to the list of tracked keys for input state monitoring.
    /// Call this before using WasKeyJustPressed or WasKeyJustReleased for a key.
    /// </summary>
    /// <param name="Key">The key to start tracking.</param>
    void add_key(char Key);

    /// <summary>
    /// This function will add the iaxis to a dynamic_array
    /// </summary>
    /// <param name="iaxis"></param>
    void add_IAxis(InputAxis iaxis);

    /// <summary>
    /// This function will look for the iaxis that has the name and get axis value
    /// based on what keys are pressed
    /// </summary>
    /// <param name="AxisName"></param>
    /// <returns></returns>
    float get_IAxis(string AxisName);
private:
    //--------Mouse-Info--------//
    MouseDetail MouseDetails;
    Vec2 MotionDirection;
    unordered_map<int, bool> MouseButtonStates;
    unordered_map<int, bool> PreviousMouseButtonStates;
    unordered_map<int, Vec2> MouseClickStartPositions;
    bool _FreezeMouseToCenter = false;
    size_t AmountOfMouseButton = 3; // the main three buttons (left, right and middle mouse button)
    KeyCodeData* MouseButtonsArray = new KeyCodeData[3];

    //--------DeltaTime&Window--------//
    float& delta_time;
    r_window* rw;
};

#endif // input_h