project(TransformationsExample)

set(HDRS
    TransformationsExample_global.h
    TransformationsExample.h
)

set(SRCS
    TransformationsExample.cpp
)

add_library(TransformationsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(TransformationsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(TransformationsExample PRIVATE TRANSFORMATIONSEXAMPLE_LIBRARY)

