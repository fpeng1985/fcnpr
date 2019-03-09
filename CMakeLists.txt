cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)

project(Ropper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(lib)
add_subdirectory(lib)
add_subdirectory(src)

add_executable(main "main.cpp")
target_link_libraries(main PUBLIC ropper)
