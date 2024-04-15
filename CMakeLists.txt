cmake_minimum_required(VERSION 3.5)

project(QtOpenGLExamples VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SUB_LIB
    CreateOpenGLWindow
    HelloTriangle
    Shaders
    Textures
    Transformations
    CoordinateSystems
    Camera
)

SUBDIRS(${SUB_LIB})

set(PROJECT_SOURCES
        main.cpp
        MainWindow.h
        MainWindow.cpp
        MainWindow.ui
)

add_executable(QtOpenGLExamples
    ${PROJECT_SOURCES}
)

target_link_libraries(QtOpenGLExamples PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${SUB_LIB})


set_target_properties(QtOpenGLExamples PROPERTIES
    WIN32_EXECUTABLE TRUE
)
