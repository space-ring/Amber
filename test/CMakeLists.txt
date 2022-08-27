cmake_minimum_required(VERSION 3.16)
project(Test)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-static")

add_executable(Test
        include/scenes/demo/DemoScene.h
        include/scenes/demo/GroundLayer.h
        include/scenes/demo/UILayer.h
        include/Sandbox.h
        src/DemoScene.cpp
        src/GroundLayer.cpp
        src/Sandbox.cpp
        src/test.cpp
        src/UILayer.cpp)

target_include_directories(Test PRIVATE include)

target_link_libraries(Test LINK_PUBLIC Engine)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
        )

add_dependencies(Test copy_assets)