#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

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
        std::uint32_t width,
        std::uint32_t height,
        PixelFormat format
    );

    std::uint32_t width() const noexcept;
    std::uint32_t height() const noexcept;
    PixelFormat format() const noexcept;

    std::size_t bytesPerChannel() const noexcept;
    std::size_t channelCount() const noexcept;
    std::size_t bytesPerPixel() const noexcept;
    std::size_t byteSize() const noexcept;

    bool hasAlpha() const noexcept;

    std::uint8_t* data() noexcept;
    const std::uint8_t* data() const noexcept;

private:
    std::uint32_t width_;
    std::uint32_t height_;
    PixelFormat format_;
    std::vector<std::uint8_t> data_;
};

} // namespace visionai::image
