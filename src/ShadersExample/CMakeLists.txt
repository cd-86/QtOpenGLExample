project(ShadersExample)

set(HDRS
    ShadersExample.h
)

set(SRCS
    ShadersExample.cpp
)

add_library(ShadersExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(ShadersExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

