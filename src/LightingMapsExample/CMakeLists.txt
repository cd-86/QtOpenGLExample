project(LightingMapsExample)

set(HDRS
    LightingMapsExample_global.h
    LightingMapsExample.h
    Camera.h
)

set(SRCS
    LightingMapsExample.cpp
)

add_library(LightingMapsExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(LightingMapsExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(LightingMapsExample PRIVATE LIGHTINGMAPSEXAMPLE_LIBRARY)

