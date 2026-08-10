#include <cassert>

#include "visionai/image/PixelBuffer.h"
#include "visionai/image/BrightnessOperation.h"

int main()
{
    using namespace visionai::image;

    PixelBuffer buf(2, 2, PixelFormat::RGB8);
    // set pixels to 10
    std::fill(buf.data(), buf.data() + buf.byteSize(), static_cast<std::uint8_t>(10));

    BrightnessOperation op(20.0f);
    auto out = op.process(buf);
    assert(out != nullptr);
    assert(out->width() == buf.width());
    assert(out->height() == buf.height());

    // first channel should be 30
    assert(out->data()[0] == 30);

    return 0;
}
