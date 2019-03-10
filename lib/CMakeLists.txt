add_subdirectory(mockturtle)
add_subdirectory(alice)

add_library(json INTERFACE)
target_include_directories(json INTERFACE  json/single_include/nlohmann/)

add_library(args INTERFACE)
target_include_directories(args INTERFACE  args-parser/include/)

string(ASCII 27 Esc)
set(GREEN "${Esc}[32m")
set(MAGENTA "${Esc}[35m")
set(RESET "${Esc}[m")

if(testing)
message(STATUS "${GREEN} * ${RESET}Using: ${MAGENTA}Catch2${RESET} for unit testing")
add_library(catch INTERFACE)
target_include_directories(catch INTERFACE  catch2/single_include/catch2)
endif()
