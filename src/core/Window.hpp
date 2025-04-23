#pragma once

#include <string>
#include <memory>
#include <vector>
#include <GLFW/glfw3.h>

namespace ShadowEngine {

class Window {
public:
    struct Properties {
        std::string Title;
        int Width;
        int Height;
        bool VSync;
        bool Fullscreen;
        std::string IconPath;
    };

    struct WindowData {
        std::string Title;
        int Width;
        int Height;
        bool VSync;
        bool Fullscreen;
        std::string IconPath;
    };

    explicit Window(const Properties& props);
    ~Window();

    bool Initialize();
    void Update();
    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;
    void Close();
    void SetVSync(bool enabled);
    void SetFullscreen(bool enabled);
    bool SetIcon(const std::string& iconPath);

private:
    GLFWwindow* m_Window;
    WindowData m_Data;
    GLFWimage m_Icon;
    std::vector<unsigned char> m_IconData;
};

} // namespace ShadowEngine 