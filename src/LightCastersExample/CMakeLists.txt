project(LightCastersExample)

set(HDRS
    LightCastersExample_global.h
    LightCastersExample.h
    Camera.h
)

set(SRCS
    LightCastersExample.cpp
)

add_library(LightCastersExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(LightCastersExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(LightCastersExample PRIVATE LIGHTCASTERSEXAMPLE_LIBRARY)

