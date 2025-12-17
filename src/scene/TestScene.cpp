#include "scene/Scene.hpp"
#include "scene/Camera.hpp"
#include "rendering/RenderSystem.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"
#include "input/InputManager.hpp"

namespace ShadowEngine {

TestScene::TestScene(Rendering::RenderSystem& renderSystem, Input::InputManager& inputManager)
    : m_RenderSystem(renderSystem)
    , m_InputManager(inputManager)
    , m_Camera(std::make_unique<SceneSystem::Camera>()) {}

void TestScene::OnAttach() {
    if (m_IsInitialized) {
        return;
    }

    // Simple cube geometry (position + color) and indices set up inside the render system.
    // The RenderSystem will own the created mesh and shader and render them every frame.

    // Cube vertices (position + color)
    std::vector<float> cubeVertices = {
        // Front face (red)
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        // Back face (green)
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        // Left face (blue)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        // Right face (yellow)
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,

        // Top face (magenta)
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,

        // Bottom face (cyan)
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f
    };

    // Cube indices
    std::vector<unsigned int> cubeIndices = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 7, 6,  6, 5, 4,
        // Left face
        8, 11, 10,  10, 9, 8,
        // Right face
        12, 13, 14,  14, 15, 12,
        // Top face
        16, 19, 18,  18, 17, 16,
        // Bottom face
        20, 21, 22,  22, 23, 20
    };

    auto mesh = m_RenderSystem.CreateMesh(cubeVertices, cubeIndices);
    auto shader = m_RenderSystem.CreateShader("shaders/basic.vert", "shaders/basic.frag");

    if (mesh && shader) {
        // Set up input mappings for movement (reusing mappings from input_demo)
        using namespace ShadowEngine::Input;

        InputMapping moveForward;
        moveForward.actionName = "MoveForward";
        moveForward.keys = {KeyCode::W, KeyCode::Up};
        m_InputManager.AddInputMapping(moveForward);

        InputMapping moveBackward;
        moveBackward.actionName = "MoveBackward";
        moveBackward.keys = {KeyCode::S, KeyCode::Down};
        m_InputManager.AddInputMapping(moveBackward);

        InputMapping moveLeft;
        moveLeft.actionName = "MoveLeft";
        moveLeft.keys = {KeyCode::A, KeyCode::Left};
        m_InputManager.AddInputMapping(moveLeft);

        InputMapping moveRight;
        moveRight.actionName = "MoveRight";
        moveRight.keys = {KeyCode::D, KeyCode::Right};
        m_InputManager.AddInputMapping(moveRight);

        // Optionally capture the cursor for FPS-style camera
        // m_InputManager.SetCursorMode(GLFW_CURSOR_DISABLED);

        m_IsInitialized = true;
    }
}

void TestScene::OnDetach() {
    // For now, rely on RenderSystem and Mesh/Shader destructors to clean up.
}

void TestScene::Update(float deltaTime) {
    if (!m_IsInitialized || !m_Camera) {
        return;
    }

    const float moveSpeed = 2.5f;
    const float mouseSensitivity = 0.05f;

    // Keyboard movement (WASD / Arrow keys via input mappings)
    if (m_InputManager.IsActionTriggered("MoveForward")) {
        m_Camera->MoveForward(moveSpeed * deltaTime);
    }
    if (m_InputManager.IsActionTriggered("MoveBackward")) {
        m_Camera->MoveForward(-moveSpeed * deltaTime);
    }
    if (m_InputManager.IsActionTriggered("MoveRight")) {
        m_Camera->MoveRight(moveSpeed * deltaTime);
    }
    if (m_InputManager.IsActionTriggered("MoveLeft")) {
        m_Camera->MoveRight(-moveSpeed * deltaTime);
    }

    // Mouse look
    double deltaX = 0.0;
    double deltaY = 0.0;
    m_InputManager.GetMouseDelta(deltaX, deltaY);

    if (deltaX != 0.0 || deltaY != 0.0) {
        m_Camera->AddYawPitch(
            static_cast<float>(deltaX) * mouseSensitivity,
            static_cast<float>(-deltaY) * mouseSensitivity // invert Y for typical mouse look
        );
    }

    // Reset camera with R if you get lost
    if (m_InputManager.IsKeyJustPressed(ShadowEngine::Input::KeyCode::R)) {
        m_Camera->Reset();
    }

    // Update view matrix in the render system
    auto view = m_Camera->GetViewMatrix();
    m_RenderSystem.SetViewMatrix(view);
}

} // namespace ShadowEngine


