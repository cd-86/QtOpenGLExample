project(CameraExample)

set(HDRS
    CameraExample_global.h
    CameraExample.h
)

set(SRCS
    CameraExample.cpp
)

add_library(CameraExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(CameraExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(CameraExample PRIVATE CAMERAEXAMPLE_LIBRARY)

