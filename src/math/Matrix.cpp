#include "math/Matrix.hpp"
#include <cmath>

namespace ShadowEngine {
namespace Math {

Matrix4 CreatePerspective(float fov, float aspect, float near, float far) {
    Matrix4 result;
    float f = 1.0f / std::tan(fov * 0.5f * 3.14159f / 180.0f);
    
    result.data[0] = f / aspect;
    result.data[1] = 0.0f;
    result.data[2] = 0.0f;
    result.data[3] = 0.0f;
    
    result.data[4] = 0.0f;
    result.data[5] = f;
    result.data[6] = 0.0f;
    result.data[7] = 0.0f;
    
    result.data[8] = 0.0f;
    result.data[9] = 0.0f;
    result.data[10] = (far + near) / (near - far);
    result.data[11] = -1.0f;
    
    result.data[12] = 0.0f;
    result.data[13] = 0.0f;
    result.data[14] = (2.0f * far * near) / (near - far);
    result.data[15] = 0.0f;
    
    return result;
}

Matrix4 CreateLookAt(float eyeX, float eyeY, float eyeZ,
                     float centerX, float centerY, float centerZ,
                     float upX, float upY, float upZ) {
    Matrix4 result;
    
    // Calculate the camera direction vectors
    float x0 = eyeX - centerX;
    float y0 = eyeY - centerY;
    float z0 = eyeZ - centerZ;
    
    float len = std::sqrt(x0 * x0 + y0 * y0 + z0 * z0);
    if (len > 0) {
        x0 /= len;
        y0 /= len;
        z0 /= len;
    }
    
    float x1 = upY * z0 - upZ * y0;
    float y1 = upZ * x0 - upX * z0;
    float z1 = upX * y0 - upY * x0;
    
    len = std::sqrt(x1 * x1 + y1 * y1 + z1 * z1);
    if (len > 0) {
        x1 /= len;
        y1 /= len;
        z1 /= len;
    }
    
    float x2 = y0 * z1 - z0 * y1;
    float y2 = z0 * x1 - x0 * z1;
    float z2 = x0 * y1 - y0 * x1;
    
    result.data[0] = x1;
    result.data[1] = x2;
    result.data[2] = x0;
    result.data[3] = 0.0f;
    
    result.data[4] = y1;
    result.data[5] = y2;
    result.data[6] = y0;
    result.data[7] = 0.0f;
    
    result.data[8] = z1;
    result.data[9] = z2;
    result.data[10] = z0;
    result.data[11] = 0.0f;
    
    result.data[12] = -(x1 * eyeX + y1 * eyeY + z1 * eyeZ);
    result.data[13] = -(x2 * eyeX + y2 * eyeY + z2 * eyeZ);
    result.data[14] = -(x0 * eyeX + y0 * eyeY + z0 * eyeZ);
    result.data[15] = 1.0f;
    
    return result;
}

Matrix4 CreateTranslation(float x, float y, float z) {
    Matrix4 result;
    result.data[12] = x;
    result.data[13] = y;
    result.data[14] = z;
    return result;
}

Matrix4 CreateRotation(float angle, float x, float y, float z) {
    Matrix4 result;
    float c = std::cos(angle * 3.14159f / 180.0f);
    float s = std::sin(angle * 3.14159f / 180.0f);
    float len = std::sqrt(x * x + y * y + z * z);
    
    if (len > 0) {
        x /= len;
        y /= len;
        z /= len;
    }
    
    result.data[0] = x * x * (1 - c) + c;
    result.data[1] = y * x * (1 - c) + z * s;
    result.data[2] = z * x * (1 - c) - y * s;
    result.data[3] = 0.0f;
    
    result.data[4] = x * y * (1 - c) - z * s;
    result.data[5] = y * y * (1 - c) + c;
    result.data[6] = z * y * (1 - c) + x * s;
    result.data[7] = 0.0f;
    
    result.data[8] = x * z * (1 - c) + y * s;
    result.data[9] = y * z * (1 - c) - x * s;
    result.data[10] = z * z * (1 - c) + c;
    result.data[11] = 0.0f;
    
    result.data[12] = 0.0f;
    result.data[13] = 0.0f;
    result.data[14] = 0.0f;
    result.data[15] = 1.0f;
    
    return result;
}

Matrix4 CreateScale(float x, float y, float z) {
    Matrix4 result;
    result.data[0] = x;
    result.data[5] = y;
    result.data[10] = z;
    return result;
}

} // namespace Math
} // namespace ShadowEngine 