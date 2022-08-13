cmake_minimum_required(VERSION 3.16)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-static")
# todo GLOB bad
file(GLOB depsources "deps/glad/src/glad.c" "deps/stb/stb_image.cpp")
file(GLOB engine_sources "src/*.cpp")

set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)

add_library(Engine ${engine_sources} ${depsources})

add_subdirectory(deps/glfw-3.3.6)

target_include_directories(Engine PUBLIC deps/glfw-3.3.6/include)
target_include_directories(Engine PUBLIC deps/glad/include)
target_include_directories(Engine PUBLIC deps/glm/include)
target_include_directories(Engine PUBLIC deps/stb/include)
target_include_directories(Engine PUBLIC include)

target_link_libraries(Engine glfw opengl32)