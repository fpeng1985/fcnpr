cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)

project(Ropper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(lib)
add_subdirectory(lib)
add_subdirectory(src)

string(ASCII 27 Esc)
set(RED "${Esc}[31m")
set(GREEN "${Esc}[32m")
set(RESET "${Esc}[m")

set(UI "library" CACHE STRING "")
set_property(CACHE UI PROPERTY STRINGS library args alice)

message(STATUS "${GREEN} * ${RESET}UI: ${UI}")

if("${UI}" STREQUAL "library")

elseif("${UI}" STREQUAL "args")
	add_executable(main "main/args_main.cpp")
	target_link_libraries(main PUBLIC ropper)
elseif("${UI}" STREQUAL "alice")

else()
	message(FATAL_ERROR "${RED} * ${RESET}Incorrect value for the UI variable")
endif()
