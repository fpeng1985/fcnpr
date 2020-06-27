cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)

project(fcnpr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(testing "Enable testing modules" ON)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

string(ASCII 27 Esc)
set(RED "${Esc}[31m")
set(GREEN "${Esc}[32m")
set(YELLOW "${Esc}[33m")
set(BLUE "${Esc}[34m")
set(RESET "${Esc}[m")
