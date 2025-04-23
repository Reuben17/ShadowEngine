#pragma once

#include "Window.hpp"
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
};

} // namespace ShadowEngine 