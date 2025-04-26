find_package(Eigen3)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_library(${PROJECT_NAME} 
    App.cpp
    Primitives.cpp
    Camera.cpp
    Renderer.cpp
    World.cpp
    Drawer.cpp
    Picture.cpp
)
target_link_libraries(${PROJECT_NAME} 
    Eigen3::Eigen
    sfml-graphics
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(../eigen)
