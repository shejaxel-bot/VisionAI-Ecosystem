#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>

#include "visionai/image/ImageDecoder.h"
#include "visionai/image/ImageDecoderRegistry.h"
#include "visionai/image/ImageLoader.h"
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

		return path.compare(path.size() - extension_.size(), extension_.size(),
							extension_) == 0;
	}

	std::shared_ptr<visionai::image::PixelBuffer>
	decode(const std::string& /*path*/) const override
	{
		return std::make_shared<visionai::image::PixelBuffer>(
			2, 2, visionai::image::PixelFormat::RGB8);
	}

private:
	std::string extension_;
};

std::filesystem::path createTestFile(const std::string& filename)
{
	const auto path = std::filesystem::temp_directory_path() / filename;
	std::ofstream file(path);
	assert(file.is_open());
	file << "test image data";
	return path;
}
class FailingDecoder final : public visionai::image::ImageDecoder {
public:
    bool supports(const std::string& path) const override
    {
        return path.size() >= 4 &&
               path.compare(path.size() - 4, 4, ".png") == 0;
    }

    std::shared_ptr<visionai::image::PixelBuffer>
    decode(const std::string& /*path*/) const override
    {
        return nullptr;
    }
};
void testMissingFile()
{
    const auto path =
        std::filesystem::temp_directory_path() /
        "visionai_missing_image_test.png";

    std::filesystem::remove(path);

    visionai::image::ImageDecoderRegistry registry;

    const auto result =
        visionai::image::ImageLoader::load(
            path.string(),
            registry
        );

    assert(!result.success());
    assert(result.asset == nullptr);
    assert(
        result.error ==
        visionai::image::ImageError::FileNotFound
    );
}
void testNonRegularFile()
{
    const auto directory =
        std::filesystem::temp_directory_path() /
        "visionai_directory_test";

    std::filesystem::create_directories(directory);

    visionai::image::ImageDecoderRegistry registry;

const auto result =
    visionai::image::ImageLoader::load(directory.string(),registry);

    assert(!result.success());
    assert(result.asset == nullptr);
    assert(
        result.error ==
        visionai::image::ImageError::UnsupportedFormat
    );

    std::filesystem::remove(directory);
}

void testExistingFileWithoutDecoder()
{
    const auto path =
        createTestFile("visionai_no_decoder_test.png");

     visionai::image::ImageDecoderRegistry registry;   
    const auto result =
        visionai::image::ImageLoader::load(path.string(), registry);

    assert(!result.success());
    assert(result.asset == nullptr);
    assert(
        result.error ==
        visionai::image::ImageError::UnsupportedFormat
    );

    std::filesystem::remove(path);
}


void testExistingFileWithDecoder()
{
    const auto path =
        createTestFile("visionai_with_decoder_test.png");

    visionai::image::ImageDecoderRegistry registry;

    auto decoder =
        std::make_shared<TestDecoder>(".png");

    registry.registerDecoder(decoder);

    const auto result =
        visionai::image::ImageLoader::load(
            path.string(),
            registry
        );

    assert(result.success());
    assert(result.asset != nullptr);
    assert(result.asset->id() == path.string());
    assert(result.asset->pixelBuffer() != nullptr);

    assert(result.asset->pixelBuffer()->width() == 2);
    assert(result.asset->pixelBuffer()->height() == 2);
    assert(
        result.asset->pixelBuffer()->format() ==
        visionai::image::PixelFormat::RGB8
    );

    std::filesystem::remove(path);
}
void testDecoderFailure()
{
    const auto path =
        createTestFile("visionai_decoder_failure_test.png");

    visionai::image::ImageDecoderRegistry registry;

    auto decoder = std::make_shared<FailingDecoder>();

    registry.registerDecoder(decoder);

    const auto result =
        visionai::image::ImageLoader::load(
            path.string(),
            registry
        );

    assert(!result.success());
    assert(result.asset == nullptr);
    assert(
        result.error ==
        visionai::image::ImageError::DecodeFailed
    );

    std::filesystem::remove(path);
}
} // namespace

int main()
{
    testMissingFile();
    testNonRegularFile();
    testExistingFileWithoutDecoder();
    testExistingFileWithDecoder();
    testDecoderFailure();

    return 0;
}