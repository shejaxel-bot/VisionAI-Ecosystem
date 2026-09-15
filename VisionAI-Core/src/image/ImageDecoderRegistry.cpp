#include "visionai/image/ImageDecoderRegistry.h"

#include <utility>

namespace visionai::image {

void ImageDecoderRegistry::registerDecoder(
    std::shared_ptr<ImageDecoder> decoder
)
{
    if (decoder) {
        decoders_.push_back(std::move(decoder));
    }
}

std::shared_ptr<ImageDecoder>
ImageDecoderRegistry::findDecoder(const std::string& path) const
{
    for (const auto& decoder : decoders_) {
        if (decoder && decoder->supports(path)) {
            return decoder;
        }
    }

    return nullptr;
}

std::size_t ImageDecoderRegistry::size() const noexcept
{
    return decoders_.size();
}

} // namespace visionai::image