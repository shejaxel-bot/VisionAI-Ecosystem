#pragma once

#include <memory>
#include <string>

#include "visionai/image/ImageDecoderRegistry.h"
#include "visionai/image/ImageLoadResult.h"

namespace visionai::image {

class ImageLoader {
public:
    // Attempt to load an image from path using the supplied decoder registry.
    static ImageLoadResult load(
        const std::string& path,
        const ImageDecoderRegistry& registry
    );
};

} // namespace visionai::image