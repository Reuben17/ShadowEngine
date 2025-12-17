# Input System Documentation

The ShadowEngine input system provides comprehensive input handling for keyboard, mouse, and gamepad devices.

## Features

- **Keyboard Input**: Full keyboard support with key press/release detection
- **Mouse Input**: Mouse movement, button clicks, and scroll wheel
- **Gamepad Support**: Button and axis input for game controllers
- **Input Mapping**: Configurable action mappings for different input devices
- **Event System**: Callback-based event handling
- **State Tracking**: Current and previous frame input state

## Basic Usage

### Initialization

The input system is automatically initialized when the engine starts:

```cpp
#include "Engine.hpp"
#include "input/InputManager.hpp"

auto& engine = ShadowEngine::Engine::GetInstance();
engine.Initialize("My Game", 1280, 720);

auto& inputManager = engine.GetInputManager();
```

### Checking Input State

```cpp
// Check if a key is currently pressed
if (inputManager.IsKeyPressed(ShadowEngine::Input::KeyCode::W)) {
    // Move forward
}

// Check if a key was just pressed this frame
if (inputManager.IsKeyJustPressed(ShadowEngine::Input::KeyCode::Space)) {
    // Jump
}

// Check mouse button
if (inputManager.IsMouseButtonPressed(ShadowEngine::Input::MouseButton::Left)) {
    // Fire weapon
}

// Get mouse position
double mouseX, mouseY;
inputManager.GetMousePosition(mouseX, mouseY);

// Get mouse movement delta
double deltaX, deltaY;
inputManager.GetMouseDelta(deltaX, deltaY);
```

### Input Mappings

Create configurable input mappings for game actions:

```cpp
// Create a movement mapping
ShadowEngine::Input::InputMapping moveForward;
moveForward.actionName = "MoveForward";
moveForward.keys = {ShadowEngine::Input::KeyCode::W, ShadowEngine::Input::KeyCode::Up};
inputManager.AddInputMapping(moveForward);

// Check if the action is triggered
if (inputManager.IsActionTriggered("MoveForward")) {
    // Move forward
}
```

### Event Callbacks

Register callbacks for input events:

```cpp
// Register a key press callback
inputManager.AddEventCallback(ShadowEngine::Input::EventType::KeyPressed,
    [](const ShadowEngine::Input::InputEvent& event) {
        std::cout << "Key pressed: " << event.keyCode << std::endl;
    });

// Register a mouse button callback
inputManager.AddEventCallback(ShadowEngine::Input::EventType::MouseButtonPressed,
    [](const ShadowEngine::Input::InputEvent& event) {
        std::cout << "Mouse clicked at (" << event.x << ", " << event.y << ")" << std::endl;
    });
```

## Input Types

### Key Codes

Common key codes available:
- `KeyCode::W`, `KeyCode::A`, `KeyCode::S`, `KeyCode::D` - Movement keys
- `KeyCode::Space` - Space bar
- `KeyCode::Escape` - Escape key
- `KeyCode::Enter` - Enter key
- `KeyCode::Up`, `KeyCode::Down`, `KeyCode::Left`, `KeyCode::Right` - Arrow keys
- `KeyCode::F1` through `KeyCode::F12` - Function keys

### Mouse Buttons

- `MouseButton::Left` - Left mouse button
- `MouseButton::Right` - Right mouse button
- `MouseButton::Middle` - Middle mouse button (scroll wheel click)

### Gamepad Buttons

- `GamepadButton::A`, `GamepadButton::B`, `GamepadButton::X`, `GamepadButton::Y`
- `GamepadButton::LeftBumper`, `GamepadButton::RightBumper`
- `GamepadButton::Start`, `GamepadButton::Back`
- `GamepadButton::DPadUp`, `GamepadButton::DPadDown`, `GamepadButton::DPadLeft`, `GamepadButton::DPadRight`

### Gamepad Axes

- `GamepadAxis::LeftX`, `GamepadAxis::LeftY` - Left stick
- `GamepadAxis::RightX`, `GamepadAxis::RightY` - Right stick
- `GamepadAxis::LeftTrigger`, `GamepadAxis::RightTrigger` - Triggers

## Event Types

- `EventType::KeyPressed` - Key was pressed
- `EventType::KeyReleased` - Key was released
- `EventType::KeyRepeated` - Key repeat (when held down)
- `EventType::MouseMoved` - Mouse moved
- `EventType::MouseButtonPressed` - Mouse button pressed
- `EventType::MouseButtonReleased` - Mouse button released
- `EventType::MouseScrolled` - Mouse wheel scrolled
- `EventType::GamepadConnected` - Gamepad connected
- `EventType::GamepadDisconnected` - Gamepad disconnected
- `EventType::GamepadButtonPressed` - Gamepad button pressed
- `EventType::GamepadButtonReleased` - Gamepad button released
- `EventType::GamepadAxisMoved` - Gamepad axis moved

## Advanced Features

### Cursor Control

```cpp
// Hide the cursor (for FPS-style games)
inputManager.SetCursorMode(GLFW_CURSOR_DISABLED);

// Show the cursor
inputManager.SetCursorMode(GLFW_CURSOR_NORMAL);

// Set cursor position
inputManager.SetCursorPosition(640, 360);
```

### Gamepad Support

```cpp
// Check if a gamepad is connected
if (inputManager.IsGamepadConnected(0)) {
    // Get axis values
    float leftStickX = inputManager.GetGamepadAxis(ShadowEngine::Input::GamepadAxis::LeftX);
    float leftStickY = inputManager.GetGamepadAxis(ShadowEngine::Input::GamepadAxis::LeftY);
    
    // Check button states
    if (inputManager.IsGamepadButtonPressed(ShadowEngine::Input::GamepadButton::A)) {
        // Jump
    }
}

// Get number of connected gamepads
int gamepadCount = inputManager.GetConnectedGamepadCount();
```

## Best Practices

1. **Use Input Mappings**: Instead of hardcoding key checks, use input mappings for better maintainability
2. **Check State in Update Loop**: Use `IsKeyPressed()` for continuous input and `IsKeyJustPressed()` for one-time actions
3. **Handle Multiple Input Devices**: Support both keyboard/mouse and gamepad for better accessibility
4. **Use Event Callbacks Sparingly**: Only use callbacks for UI events, not for game logic
5. **Reset Deltas**: Mouse and scroll deltas are automatically reset each frame

## Example: Complete Input Setup

```cpp
#include "Engine.hpp"
#include "input/InputManager.hpp"

int main() {
    auto& engine = ShadowEngine::Engine::GetInstance();
    engine.Initialize("Input Demo", 1280, 720);
    
    auto& inputManager = engine.GetInputManager();
    
    // Set up input mappings
    ShadowEngine::Input::InputMapping moveForward;
    moveForward.actionName = "MoveForward";
    moveForward.keys = {ShadowEngine::Input::KeyCode::W};
    moveForward.gamepadAxes = {ShadowEngine::Input::GamepadAxis::LeftY};
    inputManager.AddInputMapping(moveForward);
    
    // Main loop
    while (!engine.GetWindow().ShouldClose()) {
        if (inputManager.IsActionTriggered("MoveForward")) {
            // Move forward
        }
        
        if (inputManager.IsKeyPressed(ShadowEngine::Input::KeyCode::Escape)) {
            break;
        }
        
        engine.GetWindow().PollEvents();
        inputManager.Update();
    }
    
    return 0;
}
``` 