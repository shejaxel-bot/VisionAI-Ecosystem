#include "visionai/image/PixelBuffer.h"

#include <limits>
#include <stdexcept>

namespace visionai::image {

namespace {

struct PixelFormatInfo {
    std::size_t channels;
    std::size_t bytesPerChannel;
    bool hasAlpha;
};

PixelFormatInfo getPixelFormatInfo(PixelFormat format)
{
    switch (format) {
    case PixelFormat::RGB8:
        return {3, 1, false};

    case PixelFormat::RGBA8:
        return {4, 1, true};

    case PixelFormat::RGB16:
        return {3, 2, false};

    case PixelFormat::RGBA16:
        return {4, 2, true};

    case PixelFormat::RGB32F:
        return {3, 4, false};

    case PixelFormat::RGBA32F:
        return {4, 4, true};
    }

    throw std::invalid_argument("Unsupported pixel format");
}

std::size_t checkedMultiply(std::size_t a, std::size_t b)
{
    if (a != 0 &&
        b > std::numeric_limits<std::size_t>::max() / a) {
        throw std::bad_alloc();
    }

    return a * b;
}

} // namespace

PixelBuffer::PixelBuffer(
    std::uint32_t width,
    std::uint32_t height,
    PixelFormat format
)
    : width_(width),
      height_(height),
      format_(format)
{
    if (width == 0 || height == 0) {
        throw std::invalid_argument(
            "Width and height must be greater than zero"
        );
    }

    const PixelFormatInfo info = getPixelFormatInfo(format);

    const std::size_t widthSize =
        static_cast<std::size_t>(width);

    const std::size_t heightSize =
        static_cast<std::size_t>(height);

    const std::size_t bytesPerPixelValue =
        checkedMultiply(info.channels, info.bytesPerChannel);

    const std::size_t pixelCount =
        checkedMultiply(widthSize, heightSize);

    const std::size_t totalBytes =
        checkedMultiply(pixelCount, bytesPerPixelValue);

    data_.resize(totalBytes);
}

std::uint32_t PixelBuffer::width() const noexcept
{
    return width_;
}

std::uint32_t PixelBuffer::height() const noexcept
{
    return height_;
}

PixelFormat PixelBuffer::format() const noexcept
{
    return format_;
}

std::size_t PixelBuffer::bytesPerChannel() const noexcept
{
    return getPixelFormatInfo(format_).bytesPerChannel;
}

std::size_t PixelBuffer::channelCount() const noexcept
{
    return getPixelFormatInfo(format_).channels;
}

std::size_t PixelBuffer::bytesPerPixel() const noexcept
{
    const PixelFormatInfo info = getPixelFormatInfo(format_);

    return info.channels * info.bytesPerChannel;
}

std::size_t PixelBuffer::byteSize() const noexcept
{
    return data_.size();
}

bool PixelBuffer::hasAlpha() const noexcept
{
    return getPixelFormatInfo(format_).hasAlpha;
}

std::uint8_t* PixelBuffer::data() noexcept
{
    return data_.data();
}

const std::uint8_t* PixelBuffer::data() const noexcept
{
    return data_.data();
}

} // namespace visionai::image