cmake_minimum_required(VERSION 3.16)
project(Test)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-static")

file (GLOB test_sources "src/*.cpp")

add_executable(Test ${test_sources})

target_include_directories(Test PRIVATE include)

target_link_libraries(Test LINK_PUBLIC Engine)