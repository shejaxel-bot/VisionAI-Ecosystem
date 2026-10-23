#pragma once

#include <string>

#include "visionai/image/ImageDecodeResult.h"

namespace visionai::image {

class ImageDecoder {
public:
    virtual ~ImageDecoder() = default;

    virtual bool supports(const std::string& path) const = 0;

    virtual ImageDecodeResult decode(
        const std::string& path
    ) const = 0;
};

} // namespace visionai::image
