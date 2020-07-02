if(testing)
    enable_testing()
    message(STATUS "${GREEN} * ${RESET}Testing: ${BLUE}Enabled${RESET}")

    #add_executable(CellTest CellTest.cpp)
    #target_link_libraries(CellTest catch libfcnpr)
    #add_test(NAME test_cell COMMAND $<TARGET_FILE:CellTest>)

    #add_executable(ChessBoardTest ChessBoardTest.cpp)
    #target_link_libraries(ChessBoardTest catch libfcnpr)
    #add_test(NAME test_chessboard COMMAND $<TARGET_FILE:ChessBoardTest>)

    #add_executable(NetworkTest NetworkTest.cpp)
    #target_link_libraries(NetworkTest catch libfcnpr)
    #add_test(NAME test_network COMMAND $<TARGET_FILE:NetworkTest>)
    #target_compile_definitions(NetworkTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

    add_executable( LevelPlacementTest LevelPlacementTest.cpp)
    target_link_libraries(LevelPlacementTest catch libfcnpr)
    add_test(NAME test_levelplacement COMMAND $<TARGET_FILE:LevelPlacementTest>)

else()
    message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
