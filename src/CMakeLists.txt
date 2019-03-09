add_subdirectory(algorithms)

add_library(ropper INTERFACE)

target_include_directories(ropper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ropper
	INTERFACE
		algorithms
		alice
		mockturtle
	)
