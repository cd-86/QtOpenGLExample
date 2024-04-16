project(HelloTriangleExample)

set(HDRS
    HelloTriangleExample.h
)

set(HRCS
    HelloTriangleExample.cpp
)

add_library(HelloTriangleExample SHARED
    ${HDRS} ${HRCS}
)

target_link_libraries(HelloTriangleExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
