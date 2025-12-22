cmake_minimum_required(VERSION 3.30)
project(examples)

set(CMAKE_CXX_STANDARD 20)

add_executable(examples_events events.cpp)
target_link_libraries(examples_events PRIVATE amber)