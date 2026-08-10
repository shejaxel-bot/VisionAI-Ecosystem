#pragma once

#include "visionai/image/ImageOperation.h"

namespace visionai::image {

class BrightnessOperation final : public ImageOperation {
public:
    explicit BrightnessOperation(float amount);

    std::shared_ptr<PixelBuffer> process(
        const PixelBuffer& input
    ) const override;

private:
    float amount_;
};

} // namespace visionai::image
