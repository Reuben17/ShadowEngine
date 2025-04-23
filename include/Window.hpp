#pragma once

#include <string>
#include <functional>
#include <memory>
#include <GLFW/glfw3.h>

namespace ShadowEngine {

class Window {
public:
    // Window properties structure
    struct Properties {
        std::string Title;
        int Width;
        int Height;
        bool VSync;
        bool Fullscreen;
        std::string IconPath;  // Path to the icon file

        Properties(const std::string& title = "ShadowEngine",
                  int width = 1280,
                  int height = 720,
                  bool vsync = true,
                  bool fullscreen = false,
                  const std::string& iconPath = "")
            : Title(title)
            , Width(width)
            , Height(height)
            , VSync(vsync)
            , Fullscreen(fullscreen)
            , IconPath(iconPath)
        {}
    };

    // Custom deleter for GLFW window
    struct GLFWWindowDeleter {
        void operator()(GLFWwindow* window) {
            if (window) {
                glfwDestroyWindow(window);
            }
        }
    };

    explicit Window(const Properties& props);
    ~Window();

    // Prevent copying
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // Initialize the window
    bool Initialize();

    // Window operations
    void Close();
    bool ShouldClose() const;
    void MakeContextCurrent();
    void SwapBuffers();
    void PollEvents();
    void Update();

    // Getters
    int GetWidth() const { return m_Data.Width; }
    int GetHeight() const { return m_Data.Height; }
    bool IsVSync() const { return m_Data.VSync; }
    bool IsFullscreen() const { return m_Data.Fullscreen; }
    GLFWwindow* GetNativeWindow() const { return m_Window.get(); }

    // Setters
    void SetVSync(bool enabled);
    void SetFullscreen(bool enabled);
    void SetTitle(const std::string& title);
    bool SetIcon(const std::string& iconPath);

    // Event callbacks
    using EventCallbackFn = std::function<void(void*)>;
    void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }

private:
    // Window data structure
    struct WindowData {
        std::string Title;
        int Width;
        int Height;
        bool VSync;
        bool Fullscreen;
        std::string IconPath;
        EventCallbackFn EventCallback;

        WindowData(const Properties& props)
            : Title(props.Title)
            , Width(props.Width)
            , Height(props.Height)
            , VSync(props.VSync)
            , Fullscreen(props.Fullscreen)
            , IconPath(props.IconPath)
            , EventCallback(nullptr)
        {}
    };

    // GLFW window handle with custom deleter
    std::unique_ptr<GLFWwindow, GLFWWindowDeleter> m_Window;
    WindowData m_Data;

    // GLFW initialization
    static bool InitializeGLFW();
    static void ShutdownGLFW();

    // Helper methods
    bool LoadIcon(const std::string& iconPath);
};

} // namespace ShadowEngine 