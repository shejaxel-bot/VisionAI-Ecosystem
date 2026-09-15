#pragma once

#include <memory>
#include <vector>

#include "visionai/image/ImageDecoder.h"

namespace visionai::image {

class ImageDecoderRegistry {
public:
    void registerDecoder(
        std::shared_ptr<ImageDecoder> decoder
    );

    std::shared_ptr<ImageDecoder> findDecoder(
        const std::string& path
    ) const;

    std::size_t size() const noexcept;

private:
    std::vector<std::shared_ptr<ImageDecoder>> decoders_;
};

} // namespace visionai::image