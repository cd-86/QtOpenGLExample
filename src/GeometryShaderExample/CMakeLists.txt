project(GeometryShaderExample)

set(HDRS
    GeometryShaderExample_global.h
    GeometryShaderExample.h
    Camera.h
    Model.h
)

set(SRCS
    GeometryShaderExample.cpp
    Model.cpp
)

add_library(GeometryShaderExample SHARED
    ${HDRS} ${SRCS}
)
target_link_libraries(GeometryShaderExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC assimp)

target_compile_definitions(GeometryShaderExample PRIVATE GEOMETRYSHADEREXAMPLE_LIBRARY)

