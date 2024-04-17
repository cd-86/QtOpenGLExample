project(CreateOpenGLWindowExample)

set(HDRS
    CreateOpenGLWindowExample_global.h
    CreateOpenGLWindowExample.h
)

set(HRCS
    CreateOpenGLWindowExample.cpp
)

add_library(CreateOpenGLWindowExample SHARED
    ${HDRS} ${HRCS}
)

target_link_libraries(CreateOpenGLWindowExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(CreateOpenGLWindowExample PRIVATE CREATEOPENGLWINDOWEXAMPLE_LIBRARY)
