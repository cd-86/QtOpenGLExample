project(CameraExample)

set(HDRS
    CameraExample.h
)

set(SRCS
    CameraExample.cpp
)

add_library(CameraExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(CameraExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

