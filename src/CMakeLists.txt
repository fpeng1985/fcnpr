add_library(libfcnpr INTERFACE)
target_include_directories(libfcnpr INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libfcnpr INTERFACE mockturtle json args )

#file(GLOB CPPFILES *.cpp)
#add_executable(fcnpr ${CPPFILES})
#target_link_libraries(fcnpr PUBLIC libfcnpr)

