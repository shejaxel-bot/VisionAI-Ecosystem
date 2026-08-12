#include <cassert>

#include "visionai/image/PixelBuffer.h"

int main()
{
    using namespace visionai::image;

    PixelBuffer buf(2, 2, PixelFormat::RGB8);
    assert(buf.width() == 2);
    assert(buf.height() == 2);
    assert(buf.byteSize() == 2 * 2 * 3);

    return 0;
}
