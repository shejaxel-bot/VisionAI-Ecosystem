#pragma once

namespace visionai::image {

enum class ImageError {
    None,
    FileNotFound,
    PermissionDenied,
    UnsupportedFormat,
    InvalidImage,
    ImageTooLarge,
    DecodeFailed,
    OutOfMemory
};

} // namespace visionai::image
