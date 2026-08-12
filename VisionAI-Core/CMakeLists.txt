cmake_minimum_required(VERSION 3.20)

project(VisionAICore
    VERSION 0.0.2
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library
add_library(
    visionai_core
    STATIC

    src/core/logging/Logger.cpp
    src/core/config/Config.cpp
    src/core/engine/Engine.cpp

    src/image/PixelBuffer.cpp
    src/image/ImageAsset.cpp
    src/image/BrightnessOperation.cpp
    src/image/EditGraph.cpp
    src/image/ImageLoader.cpp
    src/image/ImageWriter.cpp
)

# public include directories
target_include_directories(
    visionai_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Demo app
add_executable(
    visionai-core-app
    src/main.cpp
)

target_link_libraries(
    visionai-core-app
    PRIVATE
    visionai_core
)

enable_testing()

add_subdirectory(tests)
