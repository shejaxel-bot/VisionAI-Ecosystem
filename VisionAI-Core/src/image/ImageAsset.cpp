#include "visionai/image/ImageAsset.h"

namespace visionai::image {

ImageAsset::ImageAsset(std::string id)
    : id_(std::move(id))
{
}

const std::string& ImageAsset::id() const noexcept
{
    return id_;
}

void ImageAsset::setPixelBuffer(
    std::shared_ptr<PixelBuffer> buffer
)
{
    pixelBuffer_ = std::move(buffer);
}

std::shared_ptr<PixelBuffer>
ImageAsset::pixelBuffer() const noexcept
{
    return pixelBuffer_;
}

} // namespace visionai::image
