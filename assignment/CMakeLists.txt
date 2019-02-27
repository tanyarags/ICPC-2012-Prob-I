cmake_minimum_required(VERSION 3.5)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/includes )

add_compile_options(-Wall -Wextra -Wpedantic)
add_executable(Rapyuta 
    src/maze.cpp
    src/traverse.cpp
    src/main.cpp
)

set_property(TARGET Rapyuta PROPERTY CXX_STANDARD 11) 
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
