project(CoordinateSystems)

set(HDRS
    CoordinateSystemsExample.h
)

set(SRCS
    CoordinateSystemsExample.cpp
)

add_library(CoordinateSystems SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(CoordinateSystems PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

