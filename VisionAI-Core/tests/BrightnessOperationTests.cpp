#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <stdexcept>

#include "visionai/image/BrightnessOperation.h"
#include "visionai/image/PixelBuffer.h"

using namespace visionai::image;

namespace {

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

void testRGB8PositiveBrightness()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 100;
    input.data()[1] = 150;
    input.data()[2] = 200;

    BrightnessOperation operation(0.20f);

    auto output = operation.process(input);

    assert(output != nullptr);
    assert(output->format() == PixelFormat::RGB8);

    // Expected normalized additive adjustment:
    // 100/255 + 0.20 ≈ 151
    // 150/255 + 0.20 ≈ 201
    // 200/255 + 0.20 ≈ 251

    assert(output->data()[0] == 151);
    assert(output->data()[1] == 201);
    assert(output->data()[2] == 251);
}

void testRGB8NegativeBrightness()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 100;
    input.data()[1] = 150;
    input.data()[2] = 200;

    BrightnessOperation operation(-0.20f);

    auto output = operation.process(input);

    assert(output != nullptr);

    // Expected approximately:
    // 100/255 - 0.20 ≈ 49
    // 150/255 - 0.20 ≈ 99
    // 200/255 - 0.20 ≈ 149

    assert(output->data()[0] == 49);
    assert(output->data()[1] == 99);
    assert(output->data()[2] == 149);
}

void testZeroBrightnessDoesNotChangePixels()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 50;
    input.data()[1] = 100;
    input.data()[2] = 200;

    BrightnessOperation operation(0.0f);

    auto output = operation.process(input);

    assert(output != nullptr);

    assert(output->data()[0] == 50);
    assert(output->data()[1] == 100);
    assert(output->data()[2] == 200);
}

void testBrightnessClampsToMaximum()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 230;
    input.data()[1] = 240;
    input.data()[2] = 255;

    BrightnessOperation operation(0.50f);

    auto output = operation.process(input);

    assert(output != nullptr);

    assert(output->data()[0] == 255);
    assert(output->data()[1] == 255);
    assert(output->data()[2] == 255);
}

void testBrightnessClampsToMinimum()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 0;
    input.data()[1] = 10;
    input.data()[2] = 20;

    BrightnessOperation operation(-0.50f);

    auto output = operation.process(input);

    assert(output != nullptr);

    assert(output->data()[0] == 0);
    assert(output->data()[1] == 0);
    assert(output->data()[2] == 0);
}

void testInputRemainsUnchanged()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 100;
    input.data()[1] = 150;
    input.data()[2] = 200;

    BrightnessOperation operation(0.20f);

    auto output = operation.process(input);

    assert(output != nullptr);

    // Original input must remain unchanged.
    assert(input.data()[0] == 100);
    assert(input.data()[1] == 150);
    assert(input.data()[2] == 200);
}

void testOutputDimensionsAndFormatArePreserved()
{
    PixelBuffer input(3, 2, PixelFormat::RGB8);

    BrightnessOperation operation(0.10f);

    auto output = operation.process(input);

    assert(output != nullptr);

    assert(output->width() == input.width());
    assert(output->height() == input.height());
    assert(output->format() == input.format());
    assert(output->byteSize() == input.byteSize());
}

void testRGBA8PreservesAlpha()
{
    PixelBuffer input(1, 1, PixelFormat::RGBA8);

    input.data()[0] = 100;
    input.data()[1] = 150;
    input.data()[2] = 200;
    input.data()[3] = 77;

    BrightnessOperation operation(0.20f);

    auto output = operation.process(input);

    assert(output != nullptr);
    assert(output->format() == PixelFormat::RGBA8);

    // RGB should change.
    assert(output->data()[0] == 151);
    assert(output->data()[1] == 201);
    assert(output->data()[2] == 251);

    // Alpha must remain unchanged.
    assert(output->data()[3] == 77);
}

void testRGB16IsSupported()
{
    PixelBuffer input(1, 1, PixelFormat::RGB16);

    writeUInt16(input.data() + 0, 10000);
    writeUInt16(input.data() + 2, 30000);
    writeUInt16(input.data() + 4, 50000);

    BrightnessOperation operation(0.20f);

    auto output = operation.process(input);

    assert(output != nullptr);
    assert(output->format() == PixelFormat::RGB16);

    assert(
        readUInt16(output->data() + 0) == 23107
    );

    assert(
        readUInt16(output->data() + 2) == 43107
    );

    assert(
        readUInt16(output->data() + 4) == 63107
    );
}

void testRGBA16PreservesAlpha()
{
    PixelBuffer input(1, 1, PixelFormat::RGBA16);

    writeUInt16(input.data() + 0, 10000);
    writeUInt16(input.data() + 2, 30000);
    writeUInt16(input.data() + 4, 50000);
    writeUInt16(input.data() + 6, 12345);

    BrightnessOperation operation(0.20f);

    auto output = operation.process(input);

    assert(output != nullptr);
    assert(output->format() == PixelFormat::RGBA16);

    assert(
        readUInt16(output->data() + 0) == 23107
    );

    assert(
        readUInt16(output->data() + 2) == 43107
    );

    assert(
        readUInt16(output->data() + 4) == 63107
    );

    // Alpha must remain unchanged.
    assert(
        readUInt16(output->data() + 6) == 12345
    );
}

void testRGB32FIsSupported()
{
    PixelBuffer input(1, 1, PixelFormat::RGB32F);

    writeFloat(input.data() + 0, 0.20f);
    writeFloat(input.data() + 4, 0.50f);
    writeFloat(input.data() + 8, 0.80f);

    BrightnessOperation operation(0.10f);

    auto output = operation.process(input);

    assert(output != nullptr);
    assert(output->format() == PixelFormat::RGB32F);

    assert(
        std::fabs(readFloat(output->data() + 0) - 0.30f)
        < 0.0001f
    );

    assert(
        std::fabs(readFloat(output->data() + 4) - 0.60f)
        < 0.0001f
    );

    assert(
        std::fabs(readFloat(output->data() + 8) - 0.90f)
        < 0.0001f
    );
}

void testRGBA32FPreservesAlpha()
{
    PixelBuffer input(1, 1, PixelFormat::RGBA32F);

    writeFloat(input.data() + 0, 0.20f);
    writeFloat(input.data() + 4, 0.50f);
    writeFloat(input.data() + 8, 0.80f);
    writeFloat(input.data() + 12, 0.35f);

    BrightnessOperation operation(0.10f);

    auto output = operation.process(input);

    assert(output != nullptr);
    assert(output->format() == PixelFormat::RGBA32F);

    assert(
        std::fabs(readFloat(output->data() + 0) - 0.30f)
        < 0.0001f
    );

    assert(
        std::fabs(readFloat(output->data() + 4) - 0.60f)
        < 0.0001f
    );

    assert(
        std::fabs(readFloat(output->data() + 8) - 0.90f)
        < 0.0001f
    );

    // Alpha must remain unchanged.
    assert(
    std::fabs(readFloat(output->data() + 12) - 0.35f)
    < 0.0001f
);
}

void testNaNBrightnessAmountIsRejected()
{
    bool threw = false;

    try {
        BrightnessOperation operation(
            std::numeric_limits<float>::quiet_NaN()
        );
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

void testPositiveInfinityBrightnessAmountIsRejected()
{
    bool threw = false;

    try {
        BrightnessOperation operation(
            std::numeric_limits<float>::infinity()
        );
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

void testNegativeInfinityBrightnessAmountIsRejected()
{
    bool threw = false;

    try {
        BrightnessOperation operation(
            -std::numeric_limits<float>::infinity()
        );
    }
    catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

void testVeryLargePositiveBrightnessClamps()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 0;
    input.data()[1] = 100;
    input.data()[2] = 255;

    BrightnessOperation operation(1000.0f);

    auto output = operation.process(input);

    assert(output != nullptr);

    assert(output->data()[0] == 255);
    assert(output->data()[1] == 255);
    assert(output->data()[2] == 255);
}

void testVeryLargeNegativeBrightnessClamps()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    input.data()[0] = 0;
    input.data()[1] = 100;
    input.data()[2] = 255;

    BrightnessOperation operation(-1000.0f);

    auto output = operation.process(input);

    assert(output != nullptr);

    assert(output->data()[0] == 0);
    assert(output->data()[1] == 0);
    assert(output->data()[2] == 0);
}

} // namespace

int main()
{
    testRGB8PositiveBrightness();
    testRGB8NegativeBrightness();
    testZeroBrightnessDoesNotChangePixels();

    testBrightnessClampsToMaximum();
    testBrightnessClampsToMinimum();

    testInputRemainsUnchanged();
    testOutputDimensionsAndFormatArePreserved();

    testRGBA8PreservesAlpha();
    testRGB16IsSupported();
    testRGBA16PreservesAlpha();
    testRGB32FIsSupported();
    testRGBA32FPreservesAlpha();

    testNaNBrightnessAmountIsRejected();
    testPositiveInfinityBrightnessAmountIsRejected();
    testNegativeInfinityBrightnessAmountIsRejected();

    testVeryLargePositiveBrightnessClamps();
    testVeryLargeNegativeBrightnessClamps();

    return 0;
}