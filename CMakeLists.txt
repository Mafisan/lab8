cmake_minimum_required(VERSION 3.16)
project(MeshViewerLab8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(MeshViewer
    src/main.cpp
    src/ViewerWindow.cpp
    src/OpenglRenderSystem.cpp
    src/GeometryPrimitives.cpp
)

target_include_directories(MeshViewer
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MeshViewer
    PRIVATE
        glfw
        glm::glm
        glad::glad
        OpenGL::GL
)
