# Assert the minimum cmake version
cmake_minimum_required(VERSION 3.22)

# Set the project name
project(learning_simd)

# Require a modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add executables
add_executable(sum src/sum.cpp)
add_executable(simd_sum src/simd_sum.cpp)

target_compile_options(simd_sum PRIVATE -O3 -mavx2 -fopt-info-vec-optimized)
