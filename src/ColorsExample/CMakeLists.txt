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
    ColorsExample_global.h
)

target_link_libraries(ColorsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(ColorsExample PRIVATE COLORSEXAMPLE_LIBRARY)

