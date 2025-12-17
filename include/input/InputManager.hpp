#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>
#include <GLFW/glfw3.h>

namespace ShadowEngine {
namespace Input {

// Forward declarations
class InputDevice;
class InputEvent;

// Input event types
enum class EventType {
    KeyPressed,
    KeyReleased,
    KeyRepeated,
    MouseMoved,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseScrolled,
    GamepadConnected,
    GamepadDisconnected,
    GamepadButtonPressed,
    GamepadButtonReleased,
    GamepadAxisMoved
};

// Key codes (mapping to GLFW keys)
enum class KeyCode {
    Unknown = GLFW_KEY_UNKNOWN,
    Space = GLFW_KEY_SPACE,
    Apostrophe = GLFW_KEY_APOSTROPHE,
    Comma = GLFW_KEY_COMMA,
    Minus = GLFW_KEY_MINUS,
    Period = GLFW_KEY_PERIOD,
    Slash = GLFW_KEY_SLASH,
    Key0 = GLFW_KEY_0,
    Key1 = GLFW_KEY_1,
    Key2 = GLFW_KEY_2,
    Key3 = GLFW_KEY_3,
    Key4 = GLFW_KEY_4,
    Key5 = GLFW_KEY_5,
    Key6 = GLFW_KEY_6,
    Key7 = GLFW_KEY_7,
    Key8 = GLFW_KEY_8,
    Key9 = GLFW_KEY_9,
    A = GLFW_KEY_A,
    B = GLFW_KEY_B,
    C = GLFW_KEY_C,
    D = GLFW_KEY_D,
    E = GLFW_KEY_E,
    F = GLFW_KEY_F,
    G = GLFW_KEY_G,
    H = GLFW_KEY_H,
    I = GLFW_KEY_I,
    J = GLFW_KEY_J,
    K = GLFW_KEY_K,
    L = GLFW_KEY_L,
    M = GLFW_KEY_M,
    N = GLFW_KEY_N,
    O = GLFW_KEY_O,
    P = GLFW_KEY_P,
    Q = GLFW_KEY_Q,
    R = GLFW_KEY_R,
    S = GLFW_KEY_S,
    T = GLFW_KEY_T,
    U = GLFW_KEY_U,
    V = GLFW_KEY_V,
    W = GLFW_KEY_W,
    X = GLFW_KEY_X,
    Y = GLFW_KEY_Y,
    Z = GLFW_KEY_Z,
    Escape = GLFW_KEY_ESCAPE,
    Enter = GLFW_KEY_ENTER,
    Tab = GLFW_KEY_TAB,
    Right = GLFW_KEY_RIGHT,
    Left = GLFW_KEY_LEFT,
    Down = GLFW_KEY_DOWN,
    Up = GLFW_KEY_UP,
    F1 = GLFW_KEY_F1,
    F2 = GLFW_KEY_F2,
    F3 = GLFW_KEY_F3,
    F4 = GLFW_KEY_F4,
    F5 = GLFW_KEY_F5,
    F6 = GLFW_KEY_F6,
    F7 = GLFW_KEY_F7,
    F8 = GLFW_KEY_F8,
    F9 = GLFW_KEY_F9,
    F10 = GLFW_KEY_F10,
    F11 = GLFW_KEY_F11,
    F12 = GLFW_KEY_F12
};

// Mouse button codes
enum class MouseButton {
    Left = GLFW_MOUSE_BUTTON_LEFT,
    Right = GLFW_MOUSE_BUTTON_RIGHT,
    Middle = GLFW_MOUSE_BUTTON_MIDDLE,
    Button4 = GLFW_MOUSE_BUTTON_4,
    Button5 = GLFW_MOUSE_BUTTON_5,
    Button6 = GLFW_MOUSE_BUTTON_6,
    Button7 = GLFW_MOUSE_BUTTON_7,
    Button8 = GLFW_MOUSE_BUTTON_8
};

// Gamepad button codes
enum class GamepadButton {
    A = GLFW_GAMEPAD_BUTTON_A,
    B = GLFW_GAMEPAD_BUTTON_B,
    X = GLFW_GAMEPAD_BUTTON_X,
    Y = GLFW_GAMEPAD_BUTTON_Y,
    LeftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    RightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    Back = GLFW_GAMEPAD_BUTTON_BACK,
    Start = GLFW_GAMEPAD_BUTTON_START,
    Guide = GLFW_GAMEPAD_BUTTON_GUIDE,
    LeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    RightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    DPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    DPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    DPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    DPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT
};

// Gamepad axis codes
enum class GamepadAxis {
    LeftX = GLFW_GAMEPAD_AXIS_LEFT_X,
    LeftY = GLFW_GAMEPAD_AXIS_LEFT_Y,
    RightX = GLFW_GAMEPAD_AXIS_RIGHT_X,
    RightY = GLFW_GAMEPAD_AXIS_RIGHT_Y,
    LeftTrigger = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    RightTrigger = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
};

// Input state tracking
struct InputState {
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    std::unordered_map<int, bool> gamepadButtonStates;
    std::unordered_map<int, float> gamepadAxisStates;
    
    double mouseX = 0.0;
    double mouseY = 0.0;
    double mouseDeltaX = 0.0;
    double mouseDeltaY = 0.0;
    double scrollDeltaX = 0.0;
    double scrollDeltaY = 0.0;
    
    bool IsKeyPressed(KeyCode key) const;
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsGamepadButtonPressed(GamepadButton button) const;
    float GetGamepadAxis(GamepadAxis axis) const;
};

// Input event structure
struct InputEvent {
    EventType type;
    int keyCode = 0;
    int mouseButton = 0;
    int gamepadButton = 0;
    int gamepadAxis = 0;
    double x = 0.0;
    double y = 0.0;
    double deltaX = 0.0;
    double deltaY = 0.0;
    float axisValue = 0.0f;
    int gamepadId = 0;
};

// Input mapping for configurable controls
struct InputMapping {
    std::string actionName;
    std::vector<KeyCode> keys;
    std::vector<MouseButton> mouseButtons;
    std::vector<GamepadButton> gamepadButtons;
    std::vector<GamepadAxis> gamepadAxes;
    
    bool IsTriggered(const InputState& state) const;
};

// Event callback type
using InputEventCallback = std::function<void(const InputEvent&)>;

class InputManager {
public:
    InputManager();
    ~InputManager();

    // Initialize the input system
    bool Initialize(GLFWwindow* window);
    
    // Update input state (call once per frame)
    void Update();
    
    // Event handling
    void AddEventCallback(EventType type, InputEventCallback callback);
    void RemoveEventCallback(EventType type, InputEventCallback callback);
    
    // Input state queries
    bool IsKeyPressed(KeyCode key) const;
    bool IsKeyJustPressed(KeyCode key) const;
    bool IsKeyJustReleased(KeyCode key) const;
    
    bool IsMouseButtonPressed(MouseButton button) const;
    bool IsMouseButtonJustPressed(MouseButton button) const;
    bool IsMouseButtonJustReleased(MouseButton button) const;
    
    bool IsGamepadButtonPressed(GamepadButton button) const;
    bool IsGamepadButtonJustPressed(GamepadButton button) const;
    bool IsGamepadButtonJustReleased(GamepadButton button) const;
    
    float GetGamepadAxis(GamepadAxis axis) const;
    
    // Mouse position
    void GetMousePosition(double& x, double& y) const;
    void GetMouseDelta(double& deltaX, double& deltaY) const;
    void GetScrollDelta(double& deltaX, double& deltaY) const;
    
    // Input mapping
    void AddInputMapping(const InputMapping& mapping);
    bool IsActionTriggered(const std::string& actionName) const;
    
    // Cursor control
    void SetCursorMode(int mode); // GLFW_CURSOR_NORMAL, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_DISABLED
    void SetCursorPosition(double x, double y);
    
    // Gamepad support
    bool IsGamepadConnected(int gamepadId) const;
    int GetConnectedGamepadCount() const;

private:
    GLFWwindow* m_Window;
    InputState m_CurrentState;
    InputState m_PreviousState;
    
    std::vector<InputEventCallback> m_EventCallbacks[static_cast<int>(EventType::GamepadAxisMoved) + 1];
    std::unordered_map<std::string, InputMapping> m_InputMappings;
    
    // GLFW callback functions
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void GamepadCallback(int gamepadId, int event);
    
    // Helper functions
    void ProcessEvent(const InputEvent& event);
    void UpdateGamepadStates();
};

} // namespace Input
} // namespace ShadowEngine 