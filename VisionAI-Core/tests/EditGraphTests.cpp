#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "visionai/image/BrightnessOperation.h"
#include "visionai/image/EditGraph.h"
#include "visionai/image/ImageOperation.h"
#include "visionai/image/PixelBuffer.h"

using namespace visionai::image;

class AddValueOperation final : public ImageOperation {
public:
    AddValueOperation(
        int amount,
        std::vector<int>* executionLog = nullptr,
        int operationId = 0
    )
        : amount_(amount),
          executionLog_(executionLog),
          operationId_(operationId)
    {
    }

    std::shared_ptr<PixelBuffer> process(
        const PixelBuffer& input
    ) const override
    {
        if (executionLog_ != nullptr) {
            executionLog_->push_back(operationId_);
        }

        auto output = std::make_shared<PixelBuffer>(
            input.width(),
            input.height(),
            input.format()
        );

        const auto* source = input.data();
        auto* destination = output->data();

        for (std::size_t i = 0; i < input.byteSize(); ++i) {
            const int value =
                static_cast<int>(source[i]) + amount_;

            destination[i] = static_cast<std::uint8_t>(
                std::clamp(value, 0, 255)
            );
        }

        return output;
    }

private:
    int amount_;
    std::vector<int>* executionLog_;
    int operationId_;
};

class FailingOperation final : public ImageOperation {
public:
    explicit FailingOperation(
        std::vector<int>* executionLog = nullptr
    )
        : executionLog_(executionLog)
    {
    }

    std::shared_ptr<PixelBuffer> process(
        const PixelBuffer& /*input*/
    ) const override
    {
        if (executionLog_ != nullptr) {
            executionLog_->push_back(2);
        }

        return nullptr;
    }

private:
    std::vector<int>* executionLog_;
};

void testEmptyGraphPreservesInput()
{
    PixelBuffer input(2, 2, PixelFormat::RGB8);

    std::fill(
        input.data(),
        input.data() + input.byteSize(),
        static_cast<std::uint8_t>(25)
    );

    EditGraph graph;

    auto output = graph.render(input);

    assert(output != nullptr);
    assert(output->width() == input.width());
    assert(output->height() == input.height());
    assert(output->format() == input.format());
    assert(output->byteSize() == input.byteSize());

    for (std::size_t i = 0; i < input.byteSize(); ++i) {
        assert(output->data()[i] == 25);
        assert(input.data()[i] == 25);
    }
}

void testSingleBrightnessOperation()
{
    PixelBuffer input(2, 2, PixelFormat::RGB8);

    for (std::size_t i = 0; i < input.byteSize(); ++i) {
        input.data()[i] = 10;
    }

    EditGraph graph;

    graph.addOperation(
        std::make_shared<BrightnessOperation>(0.20f)
    );

    auto output = graph.render(input);

    assert(output != nullptr);
    assert(output->width() == input.width());
    assert(output->height() == input.height());
    assert(output->format() == input.format());

    // 10 / 255 + 0.20, converted back to RGB8,
    // produces approximately 61.
    for (std::size_t i = 0; i < output->byteSize(); ++i) {
        assert(output->data()[i] == 61);
    }

    // EditGraph must preserve the original input.
    for (std::size_t i = 0; i < input.byteSize(); ++i) {
        assert(input.data()[i] == 10);
    }
}

void testMultipleOperationsExecuteInOrder()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    std::fill(
        input.data(),
        input.data() + input.byteSize(),
        static_cast<std::uint8_t>(10)
    );

    std::vector<int> executionLog;

    EditGraph graph;

    graph.addOperation(
        std::make_shared<AddValueOperation>(
            5,
            &executionLog,
            1
        )
    );

    graph.addOperation(
        std::make_shared<AddValueOperation>(
            10,
            &executionLog,
            2
        )
    );

    auto output = graph.render(input);

    assert(output != nullptr);
    assert(executionLog.size() == 2);
    assert(executionLog[0] == 1);
    assert(executionLog[1] == 2);

    assert(output->data()[0] == 25);
    assert(output->data()[1] == 25);
    assert(output->data()[2] == 25);

    assert(input.data()[0] == 10);
    assert(input.data()[1] == 10);
    assert(input.data()[2] == 10);
}

void testNullOperationIsIgnored()
{
    EditGraph graph;

    graph.addOperation(nullptr);

    assert(graph.size() == 0);
}

void testClearRemovesOperations()
{
    EditGraph graph;

    graph.addOperation(
        std::make_shared<AddValueOperation>(5)
    );

    graph.addOperation(
        std::make_shared<AddValueOperation>(10)
    );

    assert(graph.size() == 2);

    graph.clear();

    assert(graph.size() == 0);
}

void testFailureStopsPipeline()
{
    PixelBuffer input(1, 1, PixelFormat::RGB8);

    std::fill(
        input.data(),
        input.data() + input.byteSize(),
        static_cast<std::uint8_t>(10)
    );

    std::vector<int> executionLog;

    EditGraph graph;

    graph.addOperation(
        std::make_shared<AddValueOperation>(
            5,
            &executionLog,
            1
        )
    );

    graph.addOperation(
        std::make_shared<FailingOperation>(
            &executionLog
        )
    );

    graph.addOperation(
        std::make_shared<AddValueOperation>(
            100,
            &executionLog,
            3
        )
    );

    auto output = graph.render(input);

    assert(output == nullptr);
    assert(executionLog.size() == 2);
    assert(executionLog[0] == 1);
    assert(executionLog[1] == 2);
}

int main()
{
    testEmptyGraphPreservesInput();
    testSingleBrightnessOperation();
    testMultipleOperationsExecuteInOrder();
    testNullOperationIsIgnored();
    testClearRemovesOperations();
    testFailureStopsPipeline();

    return 0;
}