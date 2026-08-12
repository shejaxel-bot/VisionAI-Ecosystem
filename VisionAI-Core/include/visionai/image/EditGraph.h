#pragma once

#include <memory>
#include <vector>

#include "visionai/image/ImageOperation.h"
#include "visionai/image/PixelBuffer.h"

namespace visionai::image {

class EditGraph {
public:
    void addOperation(std::shared_ptr<ImageOperation> operation);
    void clear();
    std::shared_ptr<PixelBuffer> render(const PixelBuffer& input) const;
    std::size_t size() const noexcept;

private:
    std::vector<std::shared_ptr<ImageOperation>> operations_;
};

} // namespace visionai::image
