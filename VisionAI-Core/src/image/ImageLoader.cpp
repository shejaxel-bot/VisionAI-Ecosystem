#include "visionai/image/ImageLoader.h"

#include <filesystem>
#include <utility>

#include "visionai/image/ImageAsset.h"
#include "visionai/image/ImageLoadResult.h"

namespace visionai::image {

ImageLoadResult ImageLoader::load(
    const std::string& path,
    const ImageDecoderRegistry& registry
)
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

        auto decoder = registry.findDecoder(path);

        if (!decoder) {
            result.error = ImageError::UnsupportedFormat;
            result.message = "No decoder available for file";
            return result;
        }

        auto pixelBuffer = decoder->decode(path);

        if (!pixelBuffer) {
            result.error = ImageError::DecodeFailed;
            result.message = "Image decoder failed";
            return result;
        }

        auto asset = std::make_shared<ImageAsset>(path);

        asset->setPixelBuffer(std::move(pixelBuffer));

        result.asset = std::move(asset);

        return result;
    }
    catch (const std::exception& e) {
        result.error = ImageError::DecodeFailed;
        result.message = e.what();
        return result;
    }
}

} // namespace visionai::image