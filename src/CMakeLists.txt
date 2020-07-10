
file(GLOB CPPFILES *.cpp)
add_library(fcnpr SHARED ${CPPFILES})
set_target_properties(fcnpr PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
target_link_libraries(fcnpr mockturtle)
