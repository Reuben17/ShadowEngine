#include "../include/core/ImageLoader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../../third_party/stb/stb_image.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace ShadowEngine {

bool ImageLoader::LoadImage(const std::string& path, ImageData& outImage)
{
    // Check if file exists using fstream
    std::ifstream file(path);
    if (!file.good()) {
        std::cerr << "Image file does not exist or cannot be accessed: " << path << std::endl;
        return false;
    }
    file.close();

    // Clear output image data
    outImage.width = 0;
    outImage.height = 0;
    outImage.channels = 0;
    outImage.data.clear();

    int width, height, channels;

    // Force load as RGBA (4 channels)
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    
    if (!data)
    {
        std::cerr << "Failed to load image: " << stbi_failure_reason() << std::endl;
        return false;
    }

    // Validate image dimensions
    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid image dimensions: " << width << "x" << height << std::endl;
        stbi_image_free(data);
        return false;
    }

    try
    {
        outImage.width = width;
        outImage.height = height;
        outImage.channels = 4;  // Always 4 channels (RGBA)
        outImage.data.assign(data, data + width * height * 4);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to allocate memory for image data: " << e.what() << std::endl;
        stbi_image_free(data);
        return false;
    }
    
    stbi_image_free(data);
    return true;
}

} // namespace ShadowEngine 