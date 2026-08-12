#pragma once

#include <memory>

#include "visionai/image/PixelBuffer.h"

namespace visionai::image {

class ImageOperation {
public:
    virtual ~ImageOperation() = default;

    virtual std::shared_ptr<PixelBuffer> process(
        const PixelBuffer& input
    ) const = 0;
};

} // namespace visionai::image
