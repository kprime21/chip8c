cmake_minimum_required(VERSION 3.7)


project(out)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(out graphics.c cpu.c)
target_link_libraries(out ${SDL2_LIBRARIES})
