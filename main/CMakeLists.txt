include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(pr main.cpp)
target_link_libraries(pr PUBLIC fcnpr args)
set_target_properties(pr PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
