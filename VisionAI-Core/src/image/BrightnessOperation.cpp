#include "visionai/image/BrightnessOperation.h"

#include <algorithm>
#include <memory>

namespace visionai::image {

BrightnessOperation::BrightnessOperation(float amount)
    : amount_(amount)
{
}

std::shared_ptr<PixelBuffer>
BrightnessOperation::process(
    const PixelBuffer& input
) const
{
    if (input.format() != PixelFormat::RGB8) {
        return nullptr;
    }

    auto output = std::make_shared<PixelBuffer>(
        input.width(),
        input.height(),
        input.format()
    );

    const auto* source = input.data();
    auto* destination = output->data();

    const auto pixelCount =
        static_cast<std::size_t>(input.width()) *
        static_cast<std::size_t>(input.height());

    for (std::size_t i = 0; i < pixelCount * 3; ++i) {

        const float value =
            static_cast<float>(source[i]) + amount_;

        destination[i] =
            static_cast<std::uint8_t>(
                std::clamp(value, 0.0f, 255.0f)
            );
    }

    return output;
}

} // namespace visionai::image
