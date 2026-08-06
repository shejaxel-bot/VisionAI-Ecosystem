cmake_minimum_required(VERSION 3.16)
project(VisionAI-Core VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(visionai_core
    src/core/logging/Logger.cpp
    src/core/config/Config.cpp
    src/core/engine/Engine.cpp
)

target_include_directories(visionai_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(visionai-core-app src/main.cpp)
target_link_libraries(visionai-core-app PRIVATE visionai_core)

# Installation (optional)
install(TARGETS visionai-core-app RUNTIME DESTINATION bin)
