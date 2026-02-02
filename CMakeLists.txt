cmake_minimum_required(VERSION 3.21)
project(GridView LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(GridView
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/render.h
    ${CMAKE_CURRENT_LIST_DIR}/src/render.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/gl_renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/gl_renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/grid.h
    ${CMAKE_CURRENT_LIST_DIR}/src/grid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/overlay.h
    ${CMAKE_CURRENT_LIST_DIR}/src/overlay.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/scene.h
    ${CMAKE_CURRENT_LIST_DIR}/src/scene.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/math2d.h
    ${CMAKE_CURRENT_LIST_DIR}/src/camera2d.h
    ${CMAKE_CURRENT_LIST_DIR}/src/segment.h
)

target_link_libraries(GridView PRIVATE
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
