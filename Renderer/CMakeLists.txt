find_package(Eigen3)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} 
    App.cpp
    Camera.cpp
    Drawer.cpp
    Except.cpp
    main.cpp
    Picture.cpp
    Primitives.cpp
    Renderer.cpp
    Screen.cpp
    World.cpp
)
target_link_libraries(${PROJECT_NAME} 
    Eigen3::Eigen
    sfml-graphics
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(../eigen)
