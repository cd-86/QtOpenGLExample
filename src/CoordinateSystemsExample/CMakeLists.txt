project(CoordinateSystemsExample)

set(HDRS
    CoordinateSystemsExample_global.h
    CoordinateSystemsExample.h
)

set(SRCS
    CoordinateSystemsExample.cpp
)

add_library(CoordinateSystemsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(CoordinateSystemsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(CoordinateSystemsExample PRIVATE COORDINATESYSTEMSEXAMPLE_LIBRARY)
