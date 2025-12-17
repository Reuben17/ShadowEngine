#pragma once

#include <memory>
#include "scene/Camera.hpp"

namespace ShadowEngine {

namespace Rendering {
class RenderSystem;
}

namespace Input {
class InputManager;
}

// Base class for all scenes.
class Scene {
public:
    virtual ~Scene() = default;

    // Called once when the scene becomes active.
    virtual void OnAttach() {}

    // Called once when the scene is removed or the engine shuts down.
    virtual void OnDetach() {}

    // Called every frame before rendering.
    virtual void Update(float /*deltaTime*/) {}
};

// A simple test scene that sets up a rotating cube using the RenderSystem.
class TestScene : public Scene {
public:
    TestScene(Rendering::RenderSystem& renderSystem, Input::InputManager& inputManager);
    ~TestScene() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void Update(float deltaTime) override;

private:
    Rendering::RenderSystem& m_RenderSystem;
    Input::InputManager& m_InputManager;
    std::unique_ptr<SceneSystem::Camera> m_Camera;
    bool m_IsInitialized = false;
};

using ScenePtr = std::unique_ptr<Scene>;

} // namespace ShadowEngine
