project(TextRenderingExample)

set(HDRS
    TextRenderingExample_global.h
    TextRenderingExample.h
    FontFace.h
)

set(HRCS
    TextRenderingExample.cpp
    FontFace.cpp
)

add_library(TextRenderingExample SHARED
    ${HDRS} ${HRCS}
)

target_link_libraries(TextRenderingExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets freetype)

target_compile_definitions(TextRenderingExample PRIVATE TEXTRENDERINGEXAMPLE_LIBRARY)
