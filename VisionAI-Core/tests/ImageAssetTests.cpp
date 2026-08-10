#include <cassert>

#include "visionai/image/ImageAsset.h"
#include "visionai/image/PixelBuffer.h"

int main()
{
    using namespace visionai::image;

    ImageAsset asset("asset://01");
    auto buf = std::make_shared<PixelBuffer>(2, 2, PixelFormat::RGB8);
    asset.setPixelBuffer(buf);

    assert(asset.id() == "asset://01");
    assert(asset.pixelBuffer() == buf);

    return 0;
}
