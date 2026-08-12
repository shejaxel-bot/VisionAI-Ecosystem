#pragma once

#include <memory>
#include <string>

#include "visionai/image/PixelBuffer.h"

namespace visionai::image {

class ImageDecoder {
public:
    virtual ~ImageDecoder() = default;

    virtual bool supports(const std::string& path) const = 0;

    virtual std::shared_ptr<PixelBuffer> decode(const std::string& path) const = 0;
};

} // namespace visionai::image
