cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)

project(Ropper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_subdirectory(src)

string(ASCII 27 Esc)
set(RED "${Esc}[31m")
set(GREEN "${Esc}[32m")
set(YELLOW "${Esc}[33m")
set(BLUE "${Esc}[34m")
set(RESET "${Esc}[m")

set(UI "library" CACHE STRING "")
set_property(CACHE UI PROPERTY STRINGS library args alice)

message(STATUS "${GREEN} * ${RESET}UI: ${UI}")

if("${UI}" STREQUAL "library")

elseif("${UI}" STREQUAL "args")
	add_executable(main "main/args_main.cpp")
	target_link_libraries(main PUBLIC ropper)
elseif("${UI}" STREQUAL "alice")
	add_executable(main "main/alice_main.cpp")
	target_link_libraries(main PUBLIC ropper)
else()
	message(FATAL_ERROR "${RED} * ${RESET}Incorrect value for the UI variable")
endif()

option(testing "Enable testing modules" OFF)

if(testing)
	enable_testing()
	message(STATUS "${GREEN} * ${RESET}Testing: ${BLUE}Enabled${RESET}")
	# Area.hpp
	add_executable(test_area test/test_area.cpp)
	target_link_libraries(test_area catch ropper)
	add_test(
		NAME test_area
		COMMAND $<TARGET_FILE:test_area>
	)
else()
	message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
