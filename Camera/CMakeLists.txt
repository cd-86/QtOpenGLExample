project(Camera)

set(HDRS
    CameraExample.h
)

set(SRCS
    CameraExample.cpp
)

add_library(Camera SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(Camera PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

