project(CreateOpenGLWindow)

set(HDRS
    CreateOpenGLWindowExample.h
)

set(HRCS
    CreateOpenGLWindowExample.cpp
)

add_library(CreateOpenGLWindow SHARED
    ${HDRS} ${HRCS}
)

target_link_libraries(CreateOpenGLWindow PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
