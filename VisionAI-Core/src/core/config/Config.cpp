#include "core/config/Config.h"

#include <fstream>

namespace visionai {
namespace core {

Config::Config() noexcept : gpuEnabled_(true) {}

bool Config::isGPUEnabled() const noexcept { return gpuEnabled_; }

bool Config::loadFromFile(const std::string& path)
{
    // TODO: implement real parsing (JSON/TOML/YAML)
    std::ifstream f(path);
    if (!f) return false;
    // placeholder: keep defaults
    return true;
}

bool Config::saveToFile(const std::string& path) const
{
    std::ofstream f(path);
    if (!f) return false;
    // placeholder: write simple key
    f << "gpuEnabled=" << (gpuEnabled_ ? "1" : "0") << "\n";
    return true;
}

} // namespace core
} // namespace visionai
