#include "visionai/image/BrightnessOperation.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <stdexcept>

namespace visionai::image {

namespace {

float clampNormalized(float value)
{
    return std::clamp(value, 0.0f, 1.0f);
}

std::uint8_t adjust8Bit(
    std::uint8_t value,
    float amount
)
{
    const float normalized =
        static_cast<float>(value) / 255.0f;

    const float adjusted =
        clampNormalized(normalized + amount);

    const float scaled =
        adjusted * 255.0f;

    return static_cast<std::uint8_t>(
        std::lround(scaled)
    );
}

std::uint16_t readUInt16(
    const std::uint8_t* data
)
{
    std::uint16_t value{};

    std::memcpy(
        &value,
        data,
        sizeof(value)
    );

    return value;
}

void writeUInt16(
    std::uint8_t* data,
    std::uint16_t value
)
{
    std::memcpy(
        data,
        &value,
        sizeof(value)
    );
}

float readFloat(
    const std::uint8_t* data
)
{
    float value{};

    std::memcpy(
        &value,
        data,
        sizeof(value)
    );

    return value;
}

void writeFloat(
    std::uint8_t* data,
    float value
)
{
    std::memcpy(
        data,
        &value,
        sizeof(value)
    );
}

std::uint16_t adjust16Bit(
    std::uint16_t value,
    float amount
)
{
    constexpr float maxValue = 65535.0f;

    const float normalized =
        static_cast<float>(value) / maxValue;

    const float adjusted =
        clampNormalized(normalized + amount);

    const float scaled =
        adjusted * maxValue;

    return static_cast<std::uint16_t>(
        std::lround(scaled)
    );
}

float adjustFloat(
    float value,
    float amount
)
{
    return clampNormalized(value + amount);
}

} // namespace

BrightnessOperation::BrightnessOperation(float amount)
    : amount_(amount)
{
    if (!std::isfinite(amount_)) {
        throw std::invalid_argument(
            "Brightness amount must be finite"
        );
    }
}

std::shared_ptr<PixelBuffer>
BrightnessOperation::process(
    const PixelBuffer& input
) const
{
    auto output = std::make_shared<PixelBuffer>(
        input.width(),
        input.height(),
        input.format()
    );

    const std::size_t pixelCount =
        static_cast<std::size_t>(input.width()) *
        static_cast<std::size_t>(input.height());

    switch (input.format()) {

    case PixelFormat::RGB8:
    {
        const auto* source = input.data();
        auto* destination = output->data();

        for (std::size_t pixel = 0; pixel < pixelCount; ++pixel) {
            const std::size_t offset = pixel * 3;

            destination[offset + 0] =
                adjust8Bit(source[offset + 0], amount_);

            destination[offset + 1] =
                adjust8Bit(source[offset + 1], amount_);

            destination[offset + 2] =
                adjust8Bit(source[offset + 2], amount_);
        }

        break;
    }

    case PixelFormat::RGBA8:
    {
        const auto* source = input.data();
        auto* destination = output->data();

        for (std::size_t pixel = 0; pixel < pixelCount; ++pixel) {
            const std::size_t offset = pixel * 4;

            destination[offset + 0] =
                adjust8Bit(source[offset + 0], amount_);

            destination[offset + 1] =
                adjust8Bit(source[offset + 1], amount_);

            destination[offset + 2] =
                adjust8Bit(source[offset + 2], amount_);

            // Alpha is intentionally unchanged.
            destination[offset + 3] =
                source[offset + 3];
        }

        break;
    }

    case PixelFormat::RGB16:
    {
        const auto* source = input.data();
        auto* destination = output->data();

        constexpr std::size_t bytesPerChannel = sizeof(std::uint16_t);

        for (std::size_t pixel = 0; pixel < pixelCount; ++pixel) {
            const std::size_t pixelOffset =
                pixel * 3 * bytesPerChannel;

            const std::size_t redOffset =
                pixelOffset;

            const std::size_t greenOffset =
                pixelOffset + bytesPerChannel;

            const std::size_t blueOffset =
                pixelOffset + (2 * bytesPerChannel);

            const std::uint16_t red =
                readUInt16(source + redOffset);

            const std::uint16_t green =
                readUInt16(source + greenOffset);

            const std::uint16_t blue =
                readUInt16(source + blueOffset);

            writeUInt16(
                destination + redOffset,
                adjust16Bit(red, amount_)
            );

            writeUInt16(
                destination + greenOffset,
                adjust16Bit(green, amount_)
            );

            writeUInt16(
                destination + blueOffset,
                adjust16Bit(blue, amount_)
            );
        }

        break;
    }

    case PixelFormat::RGBA16:
    {
        const auto* source = input.data();
        auto* destination = output->data();

        constexpr std::size_t bytesPerChannel = sizeof(std::uint16_t);

        for (std::size_t pixel = 0; pixel < pixelCount; ++pixel) {
            const std::size_t pixelOffset =
                pixel * 4 * bytesPerChannel;

            const std::size_t redOffset =
                pixelOffset;

            const std::size_t greenOffset =
                pixelOffset + bytesPerChannel;

            const std::size_t blueOffset =
                pixelOffset + (2 * bytesPerChannel);

            const std::size_t alphaOffset =
                pixelOffset + (3 * bytesPerChannel);

            const std::uint16_t red =
                readUInt16(source + redOffset);

            const std::uint16_t green =
                readUInt16(source + greenOffset);

            const std::uint16_t blue =
                readUInt16(source + blueOffset);

            const std::uint16_t alpha =
                readUInt16(source + alphaOffset);

            writeUInt16(
                destination + redOffset,
                adjust16Bit(red, amount_)
            );

            writeUInt16(
                destination + greenOffset,
                adjust16Bit(green, amount_)
            );

            writeUInt16(
                destination + blueOffset,
                adjust16Bit(blue, amount_)
            );

            // Alpha is intentionally unchanged.
            writeUInt16(
                destination + alphaOffset,
                alpha
            );
        }

        break;
    }

    case PixelFormat::RGB32F:
    {
        const auto* source = input.data();
        auto* destination = output->data();

        constexpr std::size_t bytesPerChannel = sizeof(float);

        for (std::size_t pixel = 0; pixel < pixelCount; ++pixel) {
            const std::size_t pixelOffset =
                pixel * 3 * bytesPerChannel;

            const std::size_t redOffset =
                pixelOffset;

            const std::size_t greenOffset =
                pixelOffset + bytesPerChannel;

            const std::size_t blueOffset =
                pixelOffset + (2 * bytesPerChannel);

            const float red =
                readFloat(source + redOffset);

            const float green =
                readFloat(source + greenOffset);

            const float blue =
                readFloat(source + blueOffset);

            writeFloat(
                destination + redOffset,
                adjustFloat(red, amount_)
            );

            writeFloat(
                destination + greenOffset,
                adjustFloat(green, amount_)
            );

            writeFloat(
                destination + blueOffset,
                adjustFloat(blue, amount_)
            );
        }

        break;
    }

    case PixelFormat::RGBA32F:
    {
        const auto* source = input.data();
        auto* destination = output->data();

        constexpr std::size_t bytesPerChannel = sizeof(float);

        for (std::size_t pixel = 0; pixel < pixelCount; ++pixel) {
            const std::size_t pixelOffset =
                pixel * 4 * bytesPerChannel;

            const std::size_t redOffset =
                pixelOffset;

            const std::size_t greenOffset =
                pixelOffset + bytesPerChannel;

            const std::size_t blueOffset =
                pixelOffset + (2 * bytesPerChannel);

            const std::size_t alphaOffset =
                pixelOffset + (3 * bytesPerChannel);

            const float red =
                readFloat(source + redOffset);

            const float green =
                readFloat(source + greenOffset);

            const float blue =
                readFloat(source + blueOffset);

            const float alpha =
                readFloat(source + alphaOffset);

            writeFloat(
                destination + redOffset,
                adjustFloat(red, amount_)
            );

            writeFloat(
                destination + greenOffset,
                adjustFloat(green, amount_)
            );

            writeFloat(
                destination + blueOffset,
                adjustFloat(blue, amount_)
            );

            // Alpha is intentionally unchanged.
            writeFloat(
                destination + alphaOffset,
                alpha
            );
        }

        break;
    }
    }

    return output;
}

} // namespace visionai::image