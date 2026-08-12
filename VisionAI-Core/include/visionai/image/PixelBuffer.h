#pragma once

#include <cstdint>
#include <vector>
#include <stdexcept>

namespace visionai::image {

enum class PixelFormat {
    RGB8,
    RGBA8,
    RGB16,
    RGBA16,
    RGB32F,
    RGBA32F
};

class PixelBuffer {
public:
    PixelBuffer(
        uint32_t width,
        uint32_t height,
        PixelFormat format
    );

    uint32_t width() const noexcept;
    uint32_t height() const noexcept;

    PixelFormat format() const noexcept;

    std::size_t byteSize() const noexcept;

    std::uint8_t* data() noexcept;
    const std::uint8_t* data() const noexcept;

private:
    uint32_t width_;
    uint32_t height_;
    PixelFormat format_;
    std::vector<std::uint8_t> data_;
};

} // namespace visionai::image
