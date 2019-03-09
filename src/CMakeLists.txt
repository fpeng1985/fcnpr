add_subdirectory(algorithms)

add_library(ropper
	SHARED
		"Field.hpp"
		"Cell.hpp"
		"Traits.hpp"
		"Area.hpp"
		"Exception.hpp"
		"PlacementAndRouting.hpp"
		"patterns/Use.hpp"
	)


set_target_properties(ropper PROPERTIES
	LINKER_LANGUAGE
		CXX
	)

target_include_directories(ropper
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}
	)

add_executable(main "main.cpp")

target_link_libraries(ropper
	PUBLIC
		alice
		mockturtle
		algorithms
	)

target_link_libraries(main PRIVATE ropper alice)

