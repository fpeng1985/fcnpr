add_library(ropper INTERFACE)

target_include_directories(ropper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ropper
	INTERFACE
		mockturtle
		json
		args
	)

if("${UI}" STREQUAL "alice")
	add_library(stores INTERFACE)
	target_include_directories(stores INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stores")

	target_link_libraries(ropper
		INTERFACE
			alice
			stores
		)
endif()
