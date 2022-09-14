cmake_minimum_required(VERSION 3.16)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-static")
# todo GLOB bad
file(GLOB depsources "deps/glad/src/glad.c" "deps/stb/stb_image.cpp")

set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)

add_library(Engine ${depsources}
        include/AssetManager.h
        include/Camera.h
        include/CaptureMethod.h
        include/Engine.h
        include/engineIO.h
        include/engineUtils.h
        include/events.h
        include/EventManager.h
        include/Frame.h
        include/graphics.h
        include/ITransformable.h
        include/Layer.h
        include/LayerStack.h
        include/light.h
        include/maths.h
        include/Mesh.h
        include/Model.h
        include/ModelManager.h
        include/ModelTransform.h
        include/rendering.h
        include/Scene.h
        include/Shader.h
        include/Singleton.h
        include/Stage.h
        include/StateBuffer.h
        include/Texture.h
        include/Application.h
        include/Timer.h
        include/variadic.h
        src/AssetManager.cpp
        src/Camera.cpp
        src/CaptureMethod.cpp
        src/Engine.cpp
        src/engineIO.cpp
        src/engineUtils.cpp
        src/events.cpp
        src/EventManager.cpp
        src/Frame.cpp
        src/graphics.cpp
        src/Layer.cpp
        src/LayerStack.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/ModelManager.cpp
        src/ModelTransform.cpp
        src/rendering.cpp
        src/Scene.cpp
        src/Shader.cpp
        src/Stage.cpp
        src/Texture.cpp
        src/Application.cpp include/NoDefaultSingleton.h include/IApplication.h src/IApplication.cpp include/IMessage.h include/IClonable.h include/Object.h include/type_erased.h src/IMessage.cpp)

add_subdirectory(deps/glfw-3.3.6)

target_include_directories(Engine PUBLIC deps/glfw-3.3.6/include)
target_include_directories(Engine PUBLIC deps/glad/include)
target_include_directories(Engine PUBLIC deps/glm/include)
target_include_directories(Engine PUBLIC deps/stb/include)
target_include_directories(Engine PUBLIC include)

target_link_libraries(Engine glfw opengl32)