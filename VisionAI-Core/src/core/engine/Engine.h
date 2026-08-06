#ifndef VISIONAI_CORE_ENGINE_H
#define VISIONAI_CORE_ENGINE_H

#include <atomic>

namespace visionai {
namespace core {

class Engine {
public:
    Engine();
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    bool initialize();
    void shutdown();

    enum class State { Stopped, Initializing, Running, ShuttingDown, Error };
    State state() const noexcept;

private:
    std::atomic<State> state_;
};

} // namespace core
} // namespace visionai

#endif // VISIONAI_CORE_ENGINE_H
