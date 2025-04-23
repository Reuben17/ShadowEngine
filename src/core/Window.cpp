#include "../include/Window.hpp"
#include "../include/core/ImageLoader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace ShadowEngine {

Window::Window(const Properties& props)
    : m_Data(props)
{
    if (!InitializeGLFW()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

Window::~Window() {
    // No need to explicitly close - unique_ptr will handle cleanup
}

bool Window::Initialize() {
    // Create window
    GLFWwindow* window = glfwCreateWindow(
        m_Data.Width,
        m_Data.Height,
        m_Data.Title.c_str(),
        m_Data.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );

    if (!window) {
        glfwTerminate();
        return false;
    }

    // Set up the unique_ptr with the custom deleter
    m_Window = std::unique_ptr<GLFWwindow, GLFWWindowDeleter>(window);

    // Set context
    glfwMakeContextCurrent(m_Window.get());
    SetVSync(m_Data.VSync);

    // Set icon if specified
    if (!m_Data.IconPath.empty()) {
        SetIcon(m_Data.IconPath);
    }

    return true;
}

void Window::Update() {
    if (!m_Window) return;
    PollEvents();
    SwapBuffers();
}

void Window::PollEvents() {
    if (!m_Window) return;
    glfwPollEvents();
}

void Window::SwapBuffers() {
    if (!m_Window) return;
    glfwSwapBuffers(m_Window.get());
}

bool Window::ShouldClose() const {
    return m_Window ? glfwWindowShouldClose(m_Window.get()) : true;
}

void Window::Close() {
    m_Window.reset();
}

void Window::SetVSync(bool enabled) {
    if (!m_Window) return;
    m_Data.VSync = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::SetFullscreen(bool enabled) {
    if (m_Data.Fullscreen == enabled) return;

    m_Data.Fullscreen = enabled;
    if (m_Window) {
        GLFWmonitor* monitor = enabled ? glfwGetPrimaryMonitor() : nullptr;
        glfwSetWindowMonitor(m_Window.get(), monitor, 0, 0, m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
    }
}

void Window::SetTitle(const std::string& title) {
    m_Data.Title = title;
    if (m_Window) {
        glfwSetWindowTitle(m_Window.get(), title.c_str());
    }
}

bool Window::SetIcon(const std::string& iconPath) {
    if (!m_Window) return false;
    return LoadIcon(iconPath);
}

bool Window::InitializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    return true;
}

void Window::ShutdownGLFW() {
    glfwTerminate();
}

bool Window::LoadIcon(const std::string& iconPath) {
    if (!m_Window) return false;

    ImageLoader::ImageData imageData;
    if (!ImageLoader::LoadImage(iconPath, imageData)) {
        return false;
    }

    // Ensure we have RGBA data
    if (imageData.channels != 4) {
        std::cerr << "Icon must be in RGBA format" << std::endl;
        return false;
    }

    GLFWimage icon;
    icon.width = imageData.width;
    icon.height = imageData.height;
    icon.pixels = imageData.data.data();

    glfwSetWindowIcon(m_Window.get(), 1, &icon);
    return true;
}

} // namespace ShadowEngine 