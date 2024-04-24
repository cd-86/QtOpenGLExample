project(QtOpenGLExamples VERSION 0.1 LANGUAGES CXX)

INCLUDE_DIRECTORIES( ${BULLET_PHYSICS_SOURCE_DIR}/src  )

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
    MaterialsExample
    LightingMapsExample
    LightCastersExample
    MultipleLightsExample
    TextRenderingExample
    ModelLoadingExample
    PickExample
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
