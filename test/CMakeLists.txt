if(testing)
    enable_testing()
    message(STATUS "${GREEN} * ${RESET}Testing: ${BLUE}Enabled${RESET}")

    add_executable(CellTest CellTest.cpp)
    target_link_libraries(CellTest catch libfcnpr)
    add_test(NAME test_cell COMMAND $<TARGET_FILE:CellTest>)

else()
    message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
