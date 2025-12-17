#pragma once

#include <glad/glad.h>
#include "Window.hpp"
#include "rendering/RenderSystem.hpp"
#include "input/InputManager.hpp"
#include "scene/Scene.hpp"
#include <string>
#include <memory>

namespace ShadowEngine {

class Engine {
public:
    Engine();
    ~Engine();

    // Prevent copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // Initialize the engine
    bool Initialize(const std::string& windowTitle = "Shadow Engine", 
                   int windowWidth = 1280, 
                   int windowHeight = 720);

    // Shutdown the engine
    void Shutdown();

    // Main game loop
    void Run();

    // Get engine instance (singleton pattern)
    static Engine& GetInstance();

    // Window access
    Window& GetWindow() { return *m_Window; }
    const Window& GetWindow() const { return *m_Window; }

    // Render system access
    Rendering::RenderSystem& GetRenderSystem() { return *m_RenderSystem; }
    const Rendering::RenderSystem& GetRenderSystem() const { return *m_RenderSystem; }

    // Input system access
    Input::InputManager& GetInputManager() { return *m_InputManager; }
    const Input::InputManager& GetInputManager() const { return *m_InputManager; }

    // Scene management
    void SetScene(ScenePtr scene);
    Scene* GetActiveScene() { return m_Scene.get(); }
    const Scene* GetActiveScene() const { return m_Scene.get(); }

private:
    // Private initialization methods
    bool InitializeWindow();
    bool InitializeSystems();
    void ShutdownSystems();

    // Engine state
    bool m_IsInitialized;
    bool m_IsRunning;
    
    // Window parameters
    std::string m_WindowTitle;
    int m_WindowWidth;
    int m_WindowHeight;
    
    // Window
    std::unique_ptr<Window> m_Window;
    
    // Render system
    std::unique_ptr<Rendering::RenderSystem> m_RenderSystem;
    
    // Input system
    std::unique_ptr<Input::InputManager> m_InputManager;

    // Active scene
    ScenePtr m_Scene;
};

} // namespace ShadowEngine 