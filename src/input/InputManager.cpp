#include "input/InputManager.hpp"
#include <iostream>
#include <algorithm>

namespace ShadowEngine {
namespace Input {

// Static instance for GLFW callbacks
static InputManager* s_InputManagerInstance = nullptr;

// InputState methods
bool InputState::IsKeyPressed(KeyCode key) const {
    auto it = keyStates.find(static_cast<int>(key));
    return it != keyStates.end() && it->second;
}

bool InputState::IsMouseButtonPressed(MouseButton button) const {
    auto it = mouseButtonStates.find(static_cast<int>(button));
    return it != mouseButtonStates.end() && it->second;
}

bool InputState::IsGamepadButtonPressed(GamepadButton button) const {
    auto it = gamepadButtonStates.find(static_cast<int>(button));
    return it != gamepadButtonStates.end() && it->second;
}

float InputState::GetGamepadAxis(GamepadAxis axis) const {
    auto it = gamepadAxisStates.find(static_cast<int>(axis));
    return it != gamepadAxisStates.end() ? it->second : 0.0f;
}

// InputMapping methods
bool InputMapping::IsTriggered(const InputState& state) const {
    // Check keys
    for (const auto& key : keys) {
        if (state.IsKeyPressed(key)) {
            return true;
        }
    }
    
    // Check mouse buttons
    for (const auto& button : mouseButtons) {
        if (state.IsMouseButtonPressed(button)) {
            return true;
        }
    }
    
    // Check gamepad buttons
    for (const auto& button : gamepadButtons) {
        if (state.IsGamepadButtonPressed(button)) {
            return true;
        }
    }
    
    // Check gamepad axes (with deadzone)
    for (const auto& axis : gamepadAxes) {
        float value = state.GetGamepadAxis(axis);
        if (std::abs(value) > 0.1f) { // Deadzone
            return true;
        }
    }
    
    return false;
}

// InputManager implementation
InputManager::InputManager() : m_Window(nullptr) {
    s_InputManagerInstance = this;
}

InputManager::~InputManager() {
    if (s_InputManagerInstance == this) {
        s_InputManagerInstance = nullptr;
    }
}

bool InputManager::Initialize(GLFWwindow* window) {
    m_Window = window;
    
    if (!m_Window) {
        std::cerr << "InputManager: Invalid window pointer!" << std::endl;
        return false;
    }
    
    // Set GLFW callbacks
    glfwSetKeyCallback(m_Window, KeyCallback);
    glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
    glfwSetCursorPosCallback(m_Window, MousePositionCallback);
    glfwSetScrollCallback(m_Window, MouseScrollCallback);
    glfwSetJoystickCallback(GamepadCallback);
    
    // Initialize gamepad states
    UpdateGamepadStates();
    
    std::cout << "Input system initialized successfully!" << std::endl;
    return true;
}

void InputManager::Update() {
    // Store previous state
    m_PreviousState = m_CurrentState;
    
    // Reset deltas
    m_CurrentState.mouseDeltaX = 0.0;
    m_CurrentState.mouseDeltaY = 0.0;
    m_CurrentState.scrollDeltaX = 0.0;
    m_CurrentState.scrollDeltaY = 0.0;
    
    // Update gamepad states
    UpdateGamepadStates();
}

void InputManager::AddEventCallback(EventType type, InputEventCallback callback) {
    int index = static_cast<int>(type);
    if (index >= 0 && index < static_cast<int>(EventType::GamepadAxisMoved) + 1) {
        m_EventCallbacks[index].push_back(callback);
    }
}

void InputManager::RemoveEventCallback(EventType type, InputEventCallback callback) {
    // Note: std::function doesn't support equality comparison
    // This method is provided for interface completeness but doesn't work as expected
    // In a real implementation, you would need to store additional information to identify callbacks
    // For now, we'll just log that this method was called
    std::cout << "Warning: RemoveEventCallback called but std::function equality is not supported" << std::endl;
    std::cout << "Consider using a different approach for callback management" << std::endl;
}

bool InputManager::IsKeyPressed(KeyCode key) const {
    return m_CurrentState.IsKeyPressed(key);
}

bool InputManager::IsKeyJustPressed(KeyCode key) const {
    return m_CurrentState.IsKeyPressed(key) && !m_PreviousState.IsKeyPressed(key);
}

bool InputManager::IsKeyJustReleased(KeyCode key) const {
    return !m_CurrentState.IsKeyPressed(key) && m_PreviousState.IsKeyPressed(key);
}

bool InputManager::IsMouseButtonPressed(MouseButton button) const {
    return m_CurrentState.IsMouseButtonPressed(button);
}

bool InputManager::IsMouseButtonJustPressed(MouseButton button) const {
    return m_CurrentState.IsMouseButtonPressed(button) && !m_PreviousState.IsMouseButtonPressed(button);
}

bool InputManager::IsMouseButtonJustReleased(MouseButton button) const {
    return !m_CurrentState.IsMouseButtonPressed(button) && m_PreviousState.IsMouseButtonPressed(button);
}

bool InputManager::IsGamepadButtonPressed(GamepadButton button) const {
    return m_CurrentState.IsGamepadButtonPressed(button);
}

bool InputManager::IsGamepadButtonJustPressed(GamepadButton button) const {
    return m_CurrentState.IsGamepadButtonPressed(button) && !m_PreviousState.IsGamepadButtonPressed(button);
}

bool InputManager::IsGamepadButtonJustReleased(GamepadButton button) const {
    return !m_CurrentState.IsGamepadButtonPressed(button) && m_PreviousState.IsGamepadButtonPressed(button);
}

float InputManager::GetGamepadAxis(GamepadAxis axis) const {
    return m_CurrentState.GetGamepadAxis(axis);
}

void InputManager::GetMousePosition(double& x, double& y) const {
    x = m_CurrentState.mouseX;
    y = m_CurrentState.mouseY;
}

void InputManager::GetMouseDelta(double& deltaX, double& deltaY) const {
    deltaX = m_CurrentState.mouseDeltaX;
    deltaY = m_CurrentState.mouseDeltaY;
}

void InputManager::GetScrollDelta(double& deltaX, double& deltaY) const {
    deltaX = m_CurrentState.scrollDeltaX;
    deltaY = m_CurrentState.scrollDeltaY;
}

void InputManager::AddInputMapping(const InputMapping& mapping) {
    m_InputMappings[mapping.actionName] = mapping;
}

bool InputManager::IsActionTriggered(const std::string& actionName) const {
    auto it = m_InputMappings.find(actionName);
    if (it != m_InputMappings.end()) {
        return it->second.IsTriggered(m_CurrentState);
    }
    return false;
}

void InputManager::SetCursorMode(int mode) {
    if (m_Window) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, mode);
    }
}

void InputManager::SetCursorPosition(double x, double y) {
    if (m_Window) {
        glfwSetCursorPos(m_Window, x, y);
        m_CurrentState.mouseX = x;
        m_CurrentState.mouseY = y;
    }
}

bool InputManager::IsGamepadConnected(int gamepadId) const {
    return glfwJoystickPresent(gamepadId) == GLFW_TRUE;
}

int InputManager::GetConnectedGamepadCount() const {
    int count = 0;
    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i) == GLFW_TRUE) {
            count++;
        }
    }
    return count;
}

// GLFW Callback functions
void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (!s_InputManagerInstance) return;
    
    InputEvent event;
    event.keyCode = key;
    
    switch (action) {
        case GLFW_PRESS:
            event.type = EventType::KeyPressed;
            s_InputManagerInstance->m_CurrentState.keyStates[key] = true;
            break;
        case GLFW_RELEASE:
            event.type = EventType::KeyReleased;
            s_InputManagerInstance->m_CurrentState.keyStates[key] = false;
            break;
        case GLFW_REPEAT:
            event.type = EventType::KeyRepeated;
            break;
    }
    
    s_InputManagerInstance->ProcessEvent(event);
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (!s_InputManagerInstance) return;
    
    InputEvent event;
    event.mouseButton = button;
    
    switch (action) {
        case GLFW_PRESS:
            event.type = EventType::MouseButtonPressed;
            s_InputManagerInstance->m_CurrentState.mouseButtonStates[button] = true;
            break;
        case GLFW_RELEASE:
            event.type = EventType::MouseButtonReleased;
            s_InputManagerInstance->m_CurrentState.mouseButtonStates[button] = false;
            break;
    }
    
    s_InputManagerInstance->ProcessEvent(event);
}

void InputManager::MousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!s_InputManagerInstance) return;
    
    InputEvent event;
    event.type = EventType::MouseMoved;
    event.x = xpos;
    event.y = ypos;
    
    // Calculate delta
    event.deltaX = xpos - s_InputManagerInstance->m_CurrentState.mouseX;
    event.deltaY = ypos - s_InputManagerInstance->m_CurrentState.mouseY;
    
    s_InputManagerInstance->m_CurrentState.mouseX = xpos;
    s_InputManagerInstance->m_CurrentState.mouseY = ypos;
    s_InputManagerInstance->m_CurrentState.mouseDeltaX = event.deltaX;
    s_InputManagerInstance->m_CurrentState.mouseDeltaY = event.deltaY;
    
    s_InputManagerInstance->ProcessEvent(event);
}

void InputManager::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!s_InputManagerInstance) return;
    
    InputEvent event;
    event.type = EventType::MouseScrolled;
    event.deltaX = xoffset;
    event.deltaY = yoffset;
    
    s_InputManagerInstance->m_CurrentState.scrollDeltaX = xoffset;
    s_InputManagerInstance->m_CurrentState.scrollDeltaY = yoffset;
    
    s_InputManagerInstance->ProcessEvent(event);
}

void InputManager::GamepadCallback(int gamepadId, int event) {
    if (!s_InputManagerInstance) return;
    
    InputEvent inputEvent;
    inputEvent.gamepadId = gamepadId;
    
    if (event == GLFW_CONNECTED) {
        inputEvent.type = EventType::GamepadConnected;
        std::cout << "Gamepad " << gamepadId << " connected!" << std::endl;
    } else if (event == GLFW_DISCONNECTED) {
        inputEvent.type = EventType::GamepadDisconnected;
        std::cout << "Gamepad " << gamepadId << " disconnected!" << std::endl;
    }
    
    s_InputManagerInstance->ProcessEvent(inputEvent);
}

void InputManager::ProcessEvent(const InputEvent& event) {
    int index = static_cast<int>(event.type);
    if (index >= 0 && index < static_cast<int>(EventType::GamepadAxisMoved) + 1) {
        for (const auto& callback : m_EventCallbacks[index]) {
            callback(event);
        }
    }
}

void InputManager::UpdateGamepadStates() {
    for (int gamepadId = GLFW_JOYSTICK_1; gamepadId <= GLFW_JOYSTICK_LAST; ++gamepadId) {
        if (glfwJoystickPresent(gamepadId) == GLFW_TRUE) {
            GLFWgamepadstate state;
            if (glfwGetGamepadState(gamepadId, &state)) {
                // Update button states
                for (int button = 0; button < GLFW_GAMEPAD_BUTTON_LAST + 1; ++button) {
                    m_CurrentState.gamepadButtonStates[button] = state.buttons[button] == GLFW_PRESS;
                }
                
                // Update axis states
                for (int axis = 0; axis < GLFW_GAMEPAD_AXIS_LAST + 1; ++axis) {
                    m_CurrentState.gamepadAxisStates[axis] = state.axes[axis];
                }
            }
        }
    }
}

} // namespace Input
} // namespace ShadowEngine 