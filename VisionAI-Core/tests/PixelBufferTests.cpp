#include "visionai/image/PixelBuffer.h"

#include <cassert>
#include <cstdint>
#include <stdexcept>

using visionai::image::PixelBuffer;
using visionai::image::PixelFormat;

void testRGB8()
{
    PixelBuffer buffer(2, 2, PixelFormat::RGB8);

    assert(buffer.width() == 2);
    assert(buffer.height() == 2);
    assert(buffer.format() == PixelFormat::RGB8);

    assert(buffer.channelCount() == 3);
    assert(buffer.bytesPerChannel() == 1);
    assert(buffer.bytesPerPixel() == 3);
    assert(buffer.byteSize() == 12);

    assert(!buffer.hasAlpha());
}

void testRGBA8()
{
    PixelBuffer buffer(2, 2, PixelFormat::RGBA8);

    assert(buffer.channelCount() == 4);
    assert(buffer.bytesPerChannel() == 1);
    assert(buffer.bytesPerPixel() == 4);
    assert(buffer.byteSize() == 16);

    assert(buffer.hasAlpha());
}

void testRGB16()
{
    PixelBuffer buffer(2, 2, PixelFormat::RGB16);

    assert(buffer.channelCount() == 3);
    assert(buffer.bytesPerChannel() == 2);
    assert(buffer.bytesPerPixel() == 6);
    assert(buffer.byteSize() == 24);

    assert(!buffer.hasAlpha());
}

void testRGBA16()
{
    PixelBuffer buffer(2, 2, PixelFormat::RGBA16);

    assert(buffer.channelCount() == 4);
    assert(buffer.bytesPerChannel() == 2);
    assert(buffer.bytesPerPixel() == 8);
    assert(buffer.byteSize() == 32);

    assert(buffer.hasAlpha());
}

void testRGB32F()
{
    PixelBuffer buffer(2, 2, PixelFormat::RGB32F);

    assert(buffer.channelCount() == 3);
    assert(buffer.bytesPerChannel() == 4);
    assert(buffer.bytesPerPixel() == 12);
    assert(buffer.byteSize() == 48);

    assert(!buffer.hasAlpha());
}

void testRGBA32F()
{
    PixelBuffer buffer(2, 2, PixelFormat::RGBA32F);

    assert(buffer.channelCount() == 4);
    assert(buffer.bytesPerChannel() == 4);
    assert(buffer.bytesPerPixel() == 16);
    assert(buffer.byteSize() == 64);

    assert(buffer.hasAlpha());
}

void testInvalidDimensions()
{
    bool widthFailed = false;

    try {
        PixelBuffer buffer(0, 10, PixelFormat::RGB8);
    }
    catch (const std::invalid_argument&) {
        widthFailed = true;
    }

    assert(widthFailed);

    bool heightFailed = false;

    try {
        PixelBuffer buffer(10, 0, PixelFormat::RGB8);
    }
    catch (const std::invalid_argument&) {
        heightFailed = true;
    }

    assert(heightFailed);
}

void testDataAccess()
{
    PixelBuffer buffer(1, 1, PixelFormat::RGBA8);

    auto* data = buffer.data();

    data[0] = 10;
    data[1] = 20;
    data[2] = 30;
    data[3] = 255;

    const PixelBuffer& constBuffer = buffer;

    const auto* readOnlyData = constBuffer.data();

    assert(readOnlyData[0] == 10);
    assert(readOnlyData[1] == 20);
    assert(readOnlyData[2] == 30);
    assert(readOnlyData[3] == 255);
}

int main()
{
    testRGB8();
    testRGBA8();
    testRGB16();
    testRGBA16();
    testRGB32F();
    testRGBA32F();

    testInvalidDimensions();
    testDataAccess();

    return 0;
}
