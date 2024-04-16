project(TransformationsExample)

set(HDRS
    TransformationsExample.h
)

set(SRCS
    TransformationsExample.cpp
)

add_library(TransformationsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(TransformationsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

