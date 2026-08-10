#pragma once

#include <memory>
#include <string>

#include "visionai/image/PixelBuffer.h"

namespace visionai::image {

class ImageAsset {
public:
    explicit ImageAsset(std::string id);

    const std::string& id() const noexcept;

    void setPixelBuffer(
        std::shared_ptr<PixelBuffer> buffer
    );

    std::shared_ptr<PixelBuffer> pixelBuffer() const noexcept;

private:
    std::string id_;
    std::shared_ptr<PixelBuffer> pixelBuffer_;
};

} // namespace visionai::image
