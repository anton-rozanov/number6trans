cmake_minimum_required(VERSION 3.14)
project(SIX_LAB)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CTEST_OUTPUT_ON_FAILURE 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

set(VCPKG_FEATURE_FLAGS "manifests")

add_library(trans_lib lib.hpp lib.cpp)
add_executable(prog main.cpp)
target_link_libraries(prog PRIVATE trans_lib)

target_include_directories(trans_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


enable_testing()
add_subdirectory(tests)


