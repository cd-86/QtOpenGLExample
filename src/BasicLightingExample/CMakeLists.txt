project(BasicLightingExample)

set(HDRS
    BasicLightingExample_global.h
    BasicLightingExample.h
    Camera.h
)

set(SRCS
    BasicLightingExample.cpp
)

add_library(BasicLightingExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(BasicLightingExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(BasicLightingExample PRIVATE BASICLIGHTINGEXAMPLE_LIBRARY)

