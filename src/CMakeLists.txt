project(QtOpenGLExamples VERSION 0.1 LANGUAGES CXX)

set(SUB_LIB
    CreateOpenGLWindowExample
    HelloTriangleExample
    ShadersExample
    TexturesExample
    TransformationsExample
    CoordinateSystemsExample
    CameraExample
    ColorsExample
    BasicLightingExample
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
