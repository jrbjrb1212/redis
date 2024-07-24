cmake_minimum_required(VERSION 3.10)
project(redis)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 20) # Enable the C++23 standard

add_executable(server ${SOURCE_FILES})