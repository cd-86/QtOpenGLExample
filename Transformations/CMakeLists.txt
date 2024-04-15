project(Transformations)

set(HDRS
    TransformationsExample.h
)

set(SRCS
    TransformationsExample.cpp
)

add_library(Transformations SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(Transformations PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

