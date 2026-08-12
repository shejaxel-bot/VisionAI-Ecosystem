#include "visionai/image/ImageLoader.h"

#include <filesystem>

#include "visionai/image/ImageLoadResult.h"
#include "visionai/image/ImageAsset.h"

namespace visionai::image {

ImageLoadResult ImageLoader::load(const std::string& path)
{
    namespace fs = std::filesystem;

    ImageLoadResult result;

    try {
        fs::path p(path);

        if (!fs::exists(p)) {
            result.error = ImageError::FileNotFound;
            result.message = "File not found";
            return result;
        }

        if (!fs::is_regular_file(p)) {
            result.error = ImageError::UnsupportedFormat;
            result.message = "Not a regular file";
            return result;
        }

        // No decoder implemented yet. Return UnsupportedFormat until decoders are added.
        result.error = ImageError::UnsupportedFormat;
        result.message = "No decoder available in this build";
        return result;
    }
    catch (const std::exception& e) {
        result.error = ImageError::DecodeFailed;
        result.message = e.what();
        return result;
    }
}

} // namespace visionai::image
