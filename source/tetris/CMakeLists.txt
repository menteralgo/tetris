
add_library(tetris tetris.cpp objects.cpp)
target_link_libraries(tetris PRIVATE engine)