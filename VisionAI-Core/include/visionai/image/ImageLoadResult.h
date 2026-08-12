#pragma once

#include <memory>
#include <string>

#include "visionai/image/ImageAsset.h"
#include "visionai/image/ImageError.h"

namespace visionai::image {

struct ImageLoadResult {
    std::shared_ptr<ImageAsset> asset;
    ImageError error = ImageError::None;
    std::string message;

    bool success() const noexcept {
        return asset != nullptr && error == ImageError::None;
    }
};

} // namespace visionai::image
