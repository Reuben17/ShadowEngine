#pragma once

#include "math/Matrix.hpp"

namespace ShadowEngine {

namespace SceneSystem {

// Simple first-person style camera.
class Camera {
public:
    Camera()
        : m_Position{0.0f, 0.0f, 3.0f}
        , m_Yaw(-90.0f)   // Facing -Z
        , m_Pitch(0.0f) {}

    void Reset() {
        m_Position[0] = 0.0f;
        m_Position[1] = 0.0f;
        m_Position[2] = 3.0f;
        m_Yaw = -90.0f;
        m_Pitch = 0.0f;
    }

    void MoveForward(float amount);
    void MoveRight(float amount);
    void AddYawPitch(float yawOffset, float pitchOffset);

    Math::Matrix4 GetViewMatrix() const;

private:
    float m_Position[3];
    float m_Yaw;   // In degrees
    float m_Pitch; // In degrees

    void ClampPitch();
};

} // namespace SceneSystem
} // namespace ShadowEngine


