#include "visionai/image/EditGraph.h"

#include <memory>
#include <cstring>

namespace visionai::image {

void EditGraph::addOperation(std::shared_ptr<ImageOperation> operation)
{
    if (operation) operations_.push_back(std::move(operation));
}

void EditGraph::clear()
{
    operations_.clear();
}

std::size_t EditGraph::size() const noexcept
{
    return operations_.size();
}

std::shared_ptr<PixelBuffer> EditGraph::render(const PixelBuffer& input) const
{
    // Start with a copy of the input
    auto current = std::make_shared<PixelBuffer>(
        input.width(), input.height(), input.format());

    // copy data
    std::memcpy(current->data(), input.data(), input.byteSize());

    for (const auto& op : operations_) {
        if (!op) continue;

        auto next = op->process(*current);
        if (!next) return nullptr;
        current = std::move(next);
    }

    return current;
}

} // namespace visionai::image
