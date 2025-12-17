#include "Engine.hpp"
#include "input/InputManager.hpp"
#include <iostream>

int main() {
    try {
        // Get the engine instance
        auto& engine = ShadowEngine::Engine::GetInstance();

        // Initialize the engine
        if (!engine.Initialize("Input Demo", 1280, 720)) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return 1;
        }

        // Get the input manager
        auto& inputManager = engine.GetInputManager();

        // Set up input callbacks
        inputManager.AddEventCallback(ShadowEngine::Input::EventType::KeyPressed,
            [](const ShadowEngine::Input::InputEvent& event) {
                if (event.keyCode == static_cast<int>(ShadowEngine::Input::KeyCode::Escape)) {
                    std::cout << "ESC pressed - exiting!" << std::endl;
                    // In a real application, you would signal the engine to stop
                } else {
                    std::cout << "Key pressed: " << event.keyCode << std::endl;
                }
            });

        inputManager.AddEventCallback(ShadowEngine::Input::EventType::MouseButtonPressed,
            [](const ShadowEngine::Input::InputEvent& event) {
                std::cout << "Mouse button " << event.mouseButton 
                         << " pressed at (" << event.x << ", " << event.y << ")" << std::endl;
            });

        // Set up input mappings for common game actions
        ShadowEngine::Input::InputMapping moveForward;
        moveForward.actionName = "MoveForward";
        moveForward.keys = {ShadowEngine::Input::KeyCode::W, ShadowEngine::Input::KeyCode::Up};
        inputManager.AddInputMapping(moveForward);

        ShadowEngine::Input::InputMapping moveBackward;
        moveBackward.actionName = "MoveBackward";
        moveBackward.keys = {ShadowEngine::Input::KeyCode::S, ShadowEngine::Input::KeyCode::Down};
        inputManager.AddInputMapping(moveBackward);

        ShadowEngine::Input::InputMapping moveLeft;
        moveLeft.actionName = "MoveLeft";
        moveLeft.keys = {ShadowEngine::Input::KeyCode::A, ShadowEngine::Input::KeyCode::Left};
        inputManager.AddInputMapping(moveLeft);

        ShadowEngine::Input::InputMapping moveRight;
        moveRight.actionName = "MoveRight";
        moveRight.keys = {ShadowEngine::Input::KeyCode::D, ShadowEngine::Input::KeyCode::Right};
        inputManager.AddInputMapping(moveRight);

        ShadowEngine::Input::InputMapping jump;
        jump.actionName = "Jump";
        jump.keys = {ShadowEngine::Input::KeyCode::Space};
        inputManager.AddInputMapping(jump);

        // Gamepad mappings
        ShadowEngine::Input::InputMapping gamepadMove;
        gamepadMove.actionName = "GamepadMove";
        gamepadMove.gamepadAxes = {ShadowEngine::Input::GamepadAxis::LeftX, ShadowEngine::Input::GamepadAxis::LeftY};
        inputManager.AddInputMapping(gamepadMove);

        ShadowEngine::Input::InputMapping gamepadJump;
        gamepadJump.actionName = "GamepadJump";
        gamepadJump.gamepadButtons = {ShadowEngine::Input::GamepadButton::A};
        inputManager.AddInputMapping(gamepadJump);

        std::cout << "Input Demo Started!" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  WASD or Arrow Keys: Move" << std::endl;
        std::cout << "  Space: Jump" << std::endl;
        std::cout << "  Mouse: Look around" << std::endl;
        std::cout << "  Gamepad: Alternative controls" << std::endl;
        std::cout << "  ESC: Exit" << std::endl;

        // Main loop
        while (!engine.GetWindow().ShouldClose()) {
            // Check input actions
            if (inputManager.IsActionTriggered("MoveForward")) {
                std::cout << "Moving forward..." << std::endl;
            }
            if (inputManager.IsActionTriggered("MoveBackward")) {
                std::cout << "Moving backward..." << std::endl;
            }
            if (inputManager.IsActionTriggered("MoveLeft")) {
                std::cout << "Moving left..." << std::endl;
            }
            if (inputManager.IsActionTriggered("MoveRight")) {
                std::cout << "Moving right..." << std::endl;
            }
            if (inputManager.IsActionTriggered("Jump")) {
                std::cout << "Jumping!" << std::endl;
            }

            // Check for ESC key to exit
            if (inputManager.IsKeyPressed(ShadowEngine::Input::KeyCode::Escape)) {
                break;
            }

            // Process window events
            engine.GetWindow().PollEvents();
            
            // Update input system
            inputManager.Update();
        }

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 