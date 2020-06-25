add_library(libfcnpr INTERFACE)
target_include_directories(libfcnpr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libfcnpr INTERFACE mockturtle json args )

add_executable(fcnpr main.cpp)
target_link_libraries(fcnpr PUBLIC libfcnpr)

