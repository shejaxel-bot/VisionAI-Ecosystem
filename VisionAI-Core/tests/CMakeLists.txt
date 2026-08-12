cmake_minimum_required(VERSION 3.20)

project(VisionAICoreTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PixelBufferTests tests/PixelBufferTests.cpp)
target_link_libraries(PixelBufferTests PRIVATE visionai_core)
add_test(NAME PixelBufferTests COMMAND PixelBufferTests)

add_executable(ImageAssetTests tests/ImageAssetTests.cpp)
target_link_libraries(ImageAssetTests PRIVATE visionai_core)
add_test(NAME ImageAssetTests COMMAND ImageAssetTests)

add_executable(BrightnessOperationTests tests/BrightnessOperationTests.cpp)
target_link_libraries(BrightnessOperationTests PRIVATE visionai_core)
add_test(NAME BrightnessOperationTests COMMAND BrightnessOperationTests)
