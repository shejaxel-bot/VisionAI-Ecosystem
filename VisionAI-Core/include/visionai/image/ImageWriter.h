#pragma once

#include <string>

#include "visionai/image/PixelBuffer.h"

namespace visionai::image {

class ImageWriter {
public:
    // Write image to path. Returns true on success.
    static bool write(const PixelBuffer& image, const std::string& path);
};

} // namespace visionai::image
