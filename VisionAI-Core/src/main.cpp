#include "core/engine/Engine.h"
#include "version.h"

#include <iostream>

int main(int argc, char** argv)
{
    (void)argc; (void)argv;

  std::cout
    << visionai::core::NAME
    << " Version "
    << visionai::core::VERSION
    << std::endl;

    visionai::core::Engine engine;

    if (!engine.initialize()) {
        std::cerr << "Engine failed to initialize. Exiting." << std::endl;
        return 1;
    }

    std::cout << "Ready for future AI processing." << std::endl;

    engine.shutdown();

    return 0;
}
