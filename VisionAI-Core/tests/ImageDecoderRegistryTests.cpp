#include <cassert>
#include <memory>
#include <string>
#include <utility>

#include "visionai/image/ImageDecoder.h"
#include "visionai/image/ImageDecoderRegistry.h"
#include "visionai/image/PixelBuffer.h"

namespace {

class TestDecoder final : public visionai::image::ImageDecoder {
public:
    explicit TestDecoder(std::string extension)
        : extension_(std::move(extension))
    {
    }

    bool supports(const std::string& path) const override
    {
        if (path.size() < extension_.size()) {
            return false;
        }

        return path.compare(
            path.size() - extension_.size(),
            extension_.size(),
            extension_
        ) == 0;
    }

    std::shared_ptr<visionai::image::PixelBuffer>
    decode(const std::string& /*path*/) const override
    {
        return std::make_shared<visionai::image::PixelBuffer>(
            1,
            1,
            visionai::image::PixelFormat::RGB8
        );
    }

private:
    std::string extension_;
};

void testEmptyRegistry()
{
    visionai::image::ImageDecoderRegistry registry;

    assert(registry.size() == 0);
    assert(registry.findDecoder("image.png") == nullptr);
}

void testRegisterDecoder()
{
    visionai::image::ImageDecoderRegistry registry;

    auto decoder = std::make_shared<TestDecoder>(".png");

    registry.registerDecoder(decoder);

    assert(registry.size() == 1);
    assert(registry.findDecoder("image.png") == decoder);
}

void testNullDecoderIsIgnored()
{
    visionai::image::ImageDecoderRegistry registry;

    registry.registerDecoder(nullptr);

    assert(registry.size() == 0);
}

void testUnsupportedPathReturnsNull()
{
    visionai::image::ImageDecoderRegistry registry;

    auto decoder = std::make_shared<TestDecoder>(".png");

    registry.registerDecoder(decoder);

    assert(registry.findDecoder("image.jpg") == nullptr);
}

void testFirstSupportingDecoderIsReturned()
{
    visionai::image::ImageDecoderRegistry registry;

    auto pngDecoder = std::make_shared<TestDecoder>(".png");
    auto jpgDecoder = std::make_shared<TestDecoder>(".jpg");

    registry.registerDecoder(pngDecoder);
    registry.registerDecoder(jpgDecoder);

    assert(registry.findDecoder("photo.png") == pngDecoder);
    assert(registry.findDecoder("photo.jpg") == jpgDecoder);
}

} // namespace

int main()
{
    testEmptyRegistry();
    testRegisterDecoder();
    testNullDecoderIsIgnored();
    testUnsupportedPathReturnsNull();
    testFirstSupportingDecoderIsReturned();

    return 0;
}