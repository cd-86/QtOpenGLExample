project(PickExample)

set(HDRS
    PickExample_global.h
    PickExample.h
    Camera.h
    Model.h
    Line.h
    World.h
)

set(SRCS
    PickExample.cpp
    Model.cpp
    Line.cpp
    World.cpp
)

add_library(PickExample SHARED
    ${HDRS} ${SRCS}
)
target_link_libraries(PickExample
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets BulletDynamics BulletCollision LinearMath
    PUBLIC assimp
)

target_compile_definitions(PickExample PRIVATE PICKEXAMPLE_LIBRARY)

