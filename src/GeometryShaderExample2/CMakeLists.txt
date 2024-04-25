project(GeometryShaderExample2)

set(HDRS
    GeometryShaderExample2_global.h
    GeometryShaderExample2.h
    Camera.h
    Model.h
)

set(SRCS
    GeometryShaderExample2.cpp
    Model.cpp
)

add_library(GeometryShaderExample2 SHARED
    ${HDRS} ${SRCS}
)
target_link_libraries(GeometryShaderExample2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC assimp)

target_compile_definitions(GeometryShaderExample2 PRIVATE GEOMETRYSHADEREXAMPLE2_LIBRARY)

