project(HelloTriangle)

set(HDRS
    HelloTriangleExample.h
)

set(HRCS
    HelloTriangleExample.cpp
)

add_library(HelloTriangle SHARED
    ${HDRS} ${HRCS}
)

target_link_libraries(HelloTriangle PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
