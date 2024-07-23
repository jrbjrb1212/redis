cmake_minimum_required(VERSION 3.10)
project(redis)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

add_executable(server ${SOURCE_FILES})

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")