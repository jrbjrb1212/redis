cmake_minimum_required(VERSION 3.10)
project(redis)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 20) # Enable the C++23 standard
find_package(Threads REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)


add_executable(server ${SOURCE_FILES})

target_link_libraries(server PRIVATE Threads::Threads nlohmann_json::nlohmann_json)