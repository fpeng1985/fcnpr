add_library(algorithms INTERFACE)

target_include_directories(algorithms INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(algorithms INTERFACE placement/ routing/)
