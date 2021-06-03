cmake_minimum_required(VERSION 3.0)

project(Tetris)

include_directories(engine/)
include_directories(tetris/)

add_subdirectory(engine/)
add_subdirectory(tetris/)

if (UNIX)
    add_executable(Tetris game.cpp)
    target_link_libraries(Tetris PRIVATE engine tetris SDL2 SDL2_image)
endif(UNIX)