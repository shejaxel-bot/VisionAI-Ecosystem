#pragma once

#include <memory>
#include <string>

#include "visionai/image/ImageLoadResult.h"

namespace visionai::image {

class ImageLoader {
public:
    // Attempt to load an image from path. Returns a result with asset or error.
    static ImageLoadResult load(const std::string& path);
};

} // namespace visionai::image
