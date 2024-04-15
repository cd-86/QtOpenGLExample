project(Shaders)

set(HDRS
    ShadersExample.h
)

set(SRCS
    ShadersExample.cpp
)

add_library(Shaders SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(Shaders PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

