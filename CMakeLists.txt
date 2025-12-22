cmake_minimum_required(VERSION 3.30)
project(Amber LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(amber INTERFACE)
target_include_directories(amber INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(amber INTERFACE glfw)

add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE amber)

add_subdirectory(examples/events)
add_subdirectory(tests/events)

add_subdirectory(deps/glfw)