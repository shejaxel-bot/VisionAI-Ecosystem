#include "visionai/image/ImageWriter.h"

#include <iostream>

namespace visionai::image {

bool ImageWriter::write(const PixelBuffer& /*image*/, const std::string& /*path*/)
{
    // Writer not implemented yet. TODO: integrate OpenImageIO or other encoder.
    std::cerr << "ImageWriter::write: writer not implemented in this build" << std::endl;
    return false;
}

} // namespace visionai::image
