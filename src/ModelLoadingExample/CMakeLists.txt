project(ModelLoadingExample)


set(HDRS
    ModelLoadingExample_global.h
    ModelLoadingExample.h
    Camera.h
    Model.h
)

set(SRCS
    ModelLoadingExample.cpp
    Model.cpp
)

add_library(ModelLoadingExample SHARED
    ${HDRS} ${SRCS}
)

target_link_libraries(ModelLoadingExample PRIVATE Qt${QT_VERSION_MAJOR}::Widgets PUBLIC assimp)

target_compile_definitions(ModelLoadingExample PRIVATE MODELLOADINGEXAMPLE_LIBRARY)

