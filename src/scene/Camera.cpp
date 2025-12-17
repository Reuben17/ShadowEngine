#include "scene/Camera.hpp"
#include <cmath>

namespace ShadowEngine {
namespace SceneSystem {

namespace {
constexpr float DegreesToRadians(float degrees) {
    return degrees * 3.1415926535f / 180.0f;
}
}

void Camera::MoveForward(float amount) {
    float yawRad = DegreesToRadians(m_Yaw);
    float pitchRad = DegreesToRadians(m_Pitch);

    float frontX = std::cos(yawRad) * std::cos(pitchRad);
    float frontY = std::sin(pitchRad);
    float frontZ = std::sin(yawRad) * std::cos(pitchRad);

    m_Position[0] += frontX * amount;
    m_Position[1] += frontY * amount;
    m_Position[2] += frontZ * amount;
}

void Camera::MoveRight(float amount) {
    float yawRad = DegreesToRadians(m_Yaw);
    float pitchRad = DegreesToRadians(m_Pitch);

    float frontX = std::cos(yawRad) * std::cos(pitchRad);
    float frontY = std::sin(pitchRad);
    float frontZ = std::sin(yawRad) * std::cos(pitchRad);

    // Up vector
    const float upX = 0.0f;
    const float upY = 1.0f;
    const float upZ = 0.0f;

    // Right = normalize(cross(front, up))
    float rightX = frontY * upZ - frontZ * upY;
    float rightY = frontZ * upX - frontX * upZ;
    float rightZ = frontX * upY - frontY * upX;

    float len = std::sqrt(rightX * rightX + rightY * rightY + rightZ * rightZ);
    if (len > 0.0f) {
        rightX /= len;
        rightY /= len;
        rightZ /= len;
    }

    m_Position[0] += rightX * amount;
    m_Position[1] += rightY * amount;
    m_Position[2] += rightZ * amount;
}

void Camera::AddYawPitch(float yawOffset, float pitchOffset) {
    m_Yaw += yawOffset;
    m_Pitch += pitchOffset;
    ClampPitch();
}

Math::Matrix4 Camera::GetViewMatrix() const {
    float yawRad = DegreesToRadians(m_Yaw);
    float pitchRad = DegreesToRadians(m_Pitch);

    float frontX = std::cos(yawRad) * std::cos(pitchRad);
    float frontY = std::sin(pitchRad);
    float frontZ = std::sin(yawRad) * std::cos(pitchRad);

    float centerX = m_Position[0] + frontX;
    float centerY = m_Position[1] + frontY;
    float centerZ = m_Position[2] + frontZ;

    const float upX = 0.0f;
    const float upY = 1.0f;
    const float upZ = 0.0f;

    return Math::CreateLookAt(
        m_Position[0], m_Position[1], m_Position[2],
        centerX, centerY, centerZ,
        upX, upY, upZ
    );
}

void Camera::ClampPitch() {
    const float maxPitch = 89.0f;
    if (m_Pitch > maxPitch) {
        m_Pitch = maxPitch;
    }
    if (m_Pitch < -maxPitch) {
        m_Pitch = -maxPitch;
    }
}

} // namespace SceneSystem
} // namespace ShadowEngine


