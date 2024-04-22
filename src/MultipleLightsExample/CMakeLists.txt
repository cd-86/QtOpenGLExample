project(MultipleLightsExample)

set(HDRS
    MultipleLightsExample_global.h
    MultipleLightsExample.h
    Camera.h
)

set(SRCS
    MultipleLightsExample.cpp
)

add_library(MultipleLightsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(MultipleLightsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(MultipleLightsExample PRIVATE MULTIPLELIGHTSEXAMPLE_LIBRARY)

