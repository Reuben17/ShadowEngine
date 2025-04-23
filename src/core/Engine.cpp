#include "Engine.hpp"
#include <iostream>
#include <filesystem>

namespace ShadowEngine {

Engine::Engine()
    : m_IsInitialized(false)
    , m_IsRunning(false)
{
}

Engine::~Engine() {
    Shutdown();
}

Engine& Engine::GetInstance() {
    static Engine instance;
    return instance;
}

bool Engine::Initialize(const std::string& windowTitle, int windowWidth, int windowHeight) {
    if (m_IsInitialized) {
        std::cerr << "Engine already initialized!" << std::endl;
        return false;
    }

    // Store window parameters
    m_WindowTitle = windowTitle;
    m_WindowWidth = windowWidth;
    m_WindowHeight = windowHeight;

    // Create and initialize window
    if (!InitializeWindow()) {
        std::cerr << "Failed to initialize window!" << std::endl;
        return false;
    }

    // Initialize other engine systems
    if (!InitializeSystems()) {
        std::cerr << "Failed to initialize engine systems!" << std::endl;
        Shutdown();
        return false;
    }

    m_IsInitialized = true;
    return true;
}

void Engine::Shutdown() {
    if (!m_IsInitialized) {
        return;
    }

    ShutdownSystems();
    m_IsInitialized = false;
    m_IsRunning = false;
}

void Engine::Run() {
    if (!m_IsInitialized) {
        std::cerr << "Engine not initialized!" << std::endl;
        return;
    }

    m_IsRunning = true;
    std::cout << "Engine started running..." << std::endl;

    // Main game loop
    while (m_IsRunning && !m_Window->ShouldClose()) {
        // Process input
        m_Window->PollEvents();

        // Update game state
        // TODO: Implement game state updates

        // Render frame
        // TODO: Implement rendering

        // Swap buffers
        m_Window->SwapBuffers();
    }

    Shutdown();
}

bool Engine::InitializeWindow() {
    Window::Properties props;
    props.Title = m_WindowTitle;
    props.Width = m_WindowWidth;
    props.Height = m_WindowHeight;
    props.VSync = true;
    props.Fullscreen = false;
    
    // Try to find the icon in different possible locations
    std::string iconPath = "assets/icons/B.png";
    if (!std::filesystem::exists(iconPath)) {
        // Try the build directory
        iconPath = "build/windows/x64/Debug/bin/assets/icons/tree.png";
        if (!std::filesystem::exists(iconPath)) {
            // If icon not found, just continue without it
            std::cerr << "Warning: Icon file not found. Window will use default icon." << std::endl;
            iconPath.clear();
        }
    }
    props.IconPath = iconPath;

    m_Window = std::make_unique<Window>(props);
    return m_Window->Initialize();
}

bool Engine::InitializeSystems() {
    // TODO: Initialize other engine systems
    // This will be expanded as we add more systems
    std::cout << "Initializing engine systems..." << std::endl;
    return true;
}

void Engine::ShutdownSystems() {
    // TODO: Shutdown all engine systems
    // This will be expanded as we add more systems
    std::cout << "Shutting down engine systems..." << std::endl;
}

} // namespace ShadowEngine 