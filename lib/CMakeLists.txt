add_subdirectory(mockturtle)
add_subdirectory(alice)

add_library(json INTERFACE)
target_include_directories(json INTERFACE  json/single_include/nlohmann/)

add_library(args INTERFACE)
target_include_directories(args INTERFACE  args-parser/include/)
