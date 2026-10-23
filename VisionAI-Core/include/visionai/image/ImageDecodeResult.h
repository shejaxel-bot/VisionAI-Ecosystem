#pragma once

#include <memory>
#include <string>

#include "visionai/image/ImageError.h"
#include "visionai/image/PixelBuffer.h"

namespace visionai::image {

struct ImageDecodeResult {
    std::shared_ptr<PixelBuffer> pixelBuffer;
    ImageError error = ImageError::None;
    std::string message;

    bool success() const noexcept {
        return pixelBuffer != nullptr && error == ImageError::None;
    }
};

} // namespace visionai::image
