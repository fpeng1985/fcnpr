if(testing)
    enable_testing()
    message(STATUS "${GREEN} * ${RESET}Testing: ${BLUE}Enabled${RESET}")

    #add_executable(CellTest CellTest.cpp)
    #target_link_libraries(CellTest catch libfcnpr)
    #add_test(NAME test_cell COMMAND $<TARGET_FILE:CellTest>)

    add_executable(ChessBoardTest ChessBoardTest.cpp)
    target_link_libraries(ChessBoardTest catch libfcnpr)
    add_test(NAME test_chessboard COMMAND $<TARGET_FILE:ChessBoardTest>)

else()
    message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
