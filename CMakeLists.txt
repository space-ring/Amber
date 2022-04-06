cmake_minimum_required(VERSION 3.16)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-static")

file(GLOB sources "src/*.cpp" "deps/glad/src/glad.c")

add_library(Engine STATIC ${sources})
add_executable(Test ${sources} test.cpp)

#find_package(glfw3 3.3 REQUIRED)
#target_link_libraries(Engine glfw)

set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)

add_subdirectory("deps/glfw-3.3.6")

include_directories(deps/glfw-3.3.6/include)
include_directories(deps/glad/include)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include_directories(deps/glfw-3.3.6/src)
include_directories(deps/glad/src)

target_link_libraries(Engine glfw opengl32)
target_link_libraries(Test glfw opengl32 Threads::Threads)
