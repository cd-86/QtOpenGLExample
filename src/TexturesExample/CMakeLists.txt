project(TexturesExample)

set(HDRS
    TexturesExample_global.h
    TexturesExample.h
)

set(SRCS
    TexturesExample.cpp
)

add_library(TexturesExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(TexturesExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(TexturesExample PRIVATE TEXTURESEXAMPLE_LIBRARY)
