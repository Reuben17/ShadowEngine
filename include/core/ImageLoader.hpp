#pragma once

#include <string>
#include <vector>

namespace ShadowEngine {

class ImageLoader {
public:
    struct ImageData {
        int width;
        int height;
        int channels;
        std::vector<unsigned char> data;
    };

    static bool LoadImage(const std::string& path, ImageData& outImage);
};

} // namespace ShadowEngine 