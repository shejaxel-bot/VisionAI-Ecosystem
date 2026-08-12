#include "visionai/image/PixelBuffer.h"

#include <stdexcept>
#include <limits>

namespace visionai::image {

namespace {

std::size_t bytesPerPixel(PixelFormat format)
{
    switch (format) {
        case PixelFormat::RGB8:
            return 3;

        case PixelFormat::RGBA8:
            return 4;

        case PixelFormat::RGB16:
            return 6;

        case PixelFormat::RGBA16:
            return 8;

        case PixelFormat::RGB32F:
            return 12;

        case PixelFormat::RGBA32F:
            return 16;
    }

    throw std::invalid_argument("Unsupported pixel format");
}

}

PixelBuffer::PixelBuffer(
    uint32_t width,
    uint32_t height,
    PixelFormat format
)
    : width_(width),
      height_(height),
      format_(format)
{
    // basic validation
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Width and height must be > 0");
    }

    const std::size_t bpp = bytesPerPixel(format);

    // prevent overflow in multiplication
    const std::size_t maxSize = std::numeric_limits<std::size_t>::max();
    if (static_cast<std::size_t>(width) > maxSize / (static_cast<std::size_t>(height) * bpp)) {
        throw std::bad_alloc();
    }

    data_.resize(
        static_cast<std::size_t>(width) *
        static_cast<std::size_t>(height) *
        bpp
    );
}

uint32_t PixelBuffer::width() const noexcept
{
    return width_;
}

uint32_t PixelBuffer::height() const noexcept
{
    return height_;
}

PixelFormat PixelBuffer::format() const noexcept
{
    return format_;
}

std::size_t PixelBuffer::byteSize() const noexcept
{
    return data_.size();
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
