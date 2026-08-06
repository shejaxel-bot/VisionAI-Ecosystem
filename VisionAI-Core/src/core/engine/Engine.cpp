#include "core/engine/Engine.h"
#include "core/logging/Logger.h"

namespace visionai {
namespace core {

Engine::Engine() : state_(State::Stopped) {}

Engine::~Engine() { if (state_ != State::Stopped) shutdown(); }

bool Engine::initialize()
{
    state_ = State::Initializing;
    Logger::info("Starting VisionAI Core Engine...");

    // TODO: Initialize subsystems (config, memory, scheduler, plugins)

    state_ = State::Running;
    Logger::info("Engine initialized successfully.");

    return true;
}

void Engine::shutdown()
{
    State expected = state_.load();
    if (expected == State::Stopped) return;
    state_ = State::ShuttingDown;

    Logger::info("VisionAI Engine shutting down...");

    // TODO: Tear down subsystems safely

    state_ = State::Stopped;
}

Engine::State Engine::state() const noexcept { return state_.load(); }

} // namespace core
} // namespace visionai
