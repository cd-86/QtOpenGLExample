project(Textures)

set(HDRS
    TexturesExample.h
)

set(SRCS
    TexturesExample.cpp
)

add_library(Textures SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(Textures PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

