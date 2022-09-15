cmake_minimum_required(VERSION 3.24)

project(Parser)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
enable_testing()
add_subdirectory(Tests)

add_executable(Parser main.cpp)
target_link_libraries(Parser ParserLib)


