cmake_minimum_required(VERSION 3.9.6 FATAL_ERROR)

project(fcnpr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_subdirectory(src)
#add_subdirectory(test)

string(ASCII 27 Esc)
set(RED "${Esc}[31m")
set(GREEN "${Esc}[32m")
set(YELLOW "${Esc}[33m")
set(BLUE "${Esc}[34m")
set(RESET "${Esc}[m")

#set(UI "args" CACHE STRING "")
#set_property(CACHE UI PROPERTY STRINGS library args )

option(testing "Enable testing modules" ON)

if(testing)
	enable_testing()
	message(STATUS "${GREEN} * ${RESET}Testing: ${BLUE}Enabled${RESET}")
	# Area.hpp
	#add_executable(test_area test/test_area.cpp )
	#target_link_libraries(test_area catch fcnpr)
	#add_test( NAME test_area COMMAND $<TARGET_FILE:test_area> )

	#CellTest.cpp
	#add_executable(CellTest test/CellTest.cpp test/test_main.cpp)
	add_executable(CellTest test/CellTest.cpp)
	target_link_libraries(CellTest libfcnpr)
	#add_test( NAME CellTest COMMAND $<TARGET_FILE:CellTest>)

else()
	message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
