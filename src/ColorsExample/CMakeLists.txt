project(ColorsExample)

set(HDRS
    ColorsExample.h
    Camera.h
)

set(SRCS
    ColorsExample.cpp
)

add_library(ColorsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(ColorsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

