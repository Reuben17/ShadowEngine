#pragma once

#include <array>
#include <cmath>

namespace ShadowEngine {
namespace Math {

class Matrix4 {
public:
    std::array<float, 16> data;

    Matrix4() {
        // Initialize as identity matrix
        data = {1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f};
    }

    // Matrix multiplication
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.data[i * 4 + j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.data[i * 4 + j] += data[i * 4 + k] * other.data[k * 4 + j];
                }
            }
        }
        return result;
    }

    // Get pointer to data for OpenGL
    const float* GetData() const { return data.data(); }
};

// Utility functions
Matrix4 CreatePerspective(float fov, float aspect, float near, float far);
Matrix4 CreateLookAt(float eyeX, float eyeY, float eyeZ,
                     float centerX, float centerY, float centerZ,
                     float upX, float upY, float upZ);
Matrix4 CreateTranslation(float x, float y, float z);
Matrix4 CreateRotation(float angle, float x, float y, float z);
Matrix4 CreateScale(float x, float y, float z);

} // namespace Math
} // namespace ShadowEngine 