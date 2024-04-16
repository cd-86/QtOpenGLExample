project(CreateOpenGLWindowExample)

set(HDRS
    CreateOpenGLWindowExample.h
)

set(HRCS
    CreateOpenGLWindowExample.cpp
)

add_library(CreateOpenGLWindowExample SHARED
    ${HDRS} ${HRCS}
)

target_link_libraries(CreateOpenGLWindowExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
