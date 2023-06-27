cmake_minimum_required(VERSION 3.16)
project(Test)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "-static") #mingw
set(CMAKE_CXX_FLAGS) #msvc

add_executable(Test
        src/test.cpp
        include/snake.h
        src/SnakeScene.cpp
        include/scenes/demo/SnakeScene.h include/manifest.h)

target_include_directories(Test PRIVATE include)

target_link_libraries(Test LINK_PUBLIC Engine)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
        )

add_dependencies(Test copy_assets)