#include "Engine.hpp"
#include "scene/Scene.hpp"
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
void Engine::SetScene(ScenePtr scene) {
    if (m_Scene) {
        m_Scene->OnDetach();
    }
    m_Scene = std::move(scene);
    if (m_Scene) {
        m_Scene->OnAttach();
    }
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

    double lastTime = glfwGetTime();

    // Main game loop
    while (m_IsRunning && !m_Window->ShouldClose()) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // Begin new input frame (copies previous state, resets deltas)
        if (m_InputManager) {
            m_InputManager->Update();
        }

        // Process input events (mouse move, keys, etc.)
        m_Window->PollEvents();

        // Update active scene (game state)
        if (m_Scene) {
            m_Scene->Update(deltaTime);
        }

        // Render frame
        if (m_RenderSystem) {
            m_RenderSystem->Render();
        }

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
    std::string iconPath = "C:/ShadowEngine/assets/icons/B.png";
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
    // Initialize render system
    m_RenderSystem = std::make_unique<Rendering::RenderSystem>();
    if (!m_RenderSystem->Initialize(m_Window->GetNativeWindow())) {
        std::cerr << "Failed to initialize render system!" << std::endl;
        return false;
    }
    
    // Initialize input system
    m_InputManager = std::make_unique<Input::InputManager>();
    if (!m_InputManager->Initialize(m_Window->GetNativeWindow())) {
        std::cerr << "Failed to initialize input system!" << std::endl;
        return false;
    }

    // Create a default test scene so that something is visible by default.
    m_Scene = std::make_unique<TestScene>(*m_RenderSystem, *m_InputManager);
    if (m_Scene) {
        m_Scene->OnAttach();
    }

    std::cout << "Initializing engine systems..." << std::endl;
    return true;
}

void Engine::ShutdownSystems() {
    // Shutdown input system
    if (m_InputManager) {
        m_InputManager.reset();
    }
    
    // Shutdown scene
    if (m_Scene) {
        m_Scene->OnDetach();
        m_Scene.reset();
    }

    // Shutdown render system
    if (m_RenderSystem) {
        m_RenderSystem.reset();
    }
    
    std::cout << "Shutting down engine systems..." << std::endl;
}

} // namespace ShadowEngine 