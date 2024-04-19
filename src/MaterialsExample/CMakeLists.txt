project(MaterialsExample)

set(HDRS
    MaterialsExample_global.h
    MaterialsExample.h
    Camera.h
)

set(SRCS
    MaterialsExample.cpp
)

add_library(MaterialsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(MaterialsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(MaterialsExample PRIVATE MATERIALSEXAMPLE_LIBRARY)

