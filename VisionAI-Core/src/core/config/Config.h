#ifndef VISIONAI_CORE_CONFIG_H
#define VISIONAI_CORE_CONFIG_H

#include <string>

namespace visionai {
namespace core {

class Config {
public:
    Config() noexcept;

    bool isGPUEnabled() const noexcept;

    // Future: load/save from file
    bool loadFromFile(const std::string& path);
    bool saveToFile(const std::string& path) const;

private:
    bool gpuEnabled_;
};

} // namespace core
} // namespace visionai

#endif // VISIONAI_CORE_CONFIG_H
