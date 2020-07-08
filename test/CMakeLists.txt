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

    #add_executable( LevelPlacementTest LevelPlacementTest.cpp)
    #target_link_libraries(LevelPlacementTest catch libfcnpr)
    #add_test(NAME test_levelplacement COMMAND $<TARGET_FILE:LevelPlacementTest>)
    #target_compile_definitions(LevelPlacementTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

    #add_executable(LevelRoutingTest LevelRoutingTest.cpp)
    #target_link_libraries(LevelRoutingTest catch libfcnpr)
    #add_test(NAME test_levelrouting COMMAND $<TARGET_FILE:LevelRoutingTest>)
    #target_compile_definitions(LevelRoutingTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

    #add_executable( SolutionTest SolutionTest.cpp)
    #target_link_libraries(SolutionTest catch libfcnpr)
    #add_test(NAME test_solution COMMAND $<TARGET_FILE:SolutionTest>)
    #target_compile_definitions(SolutionTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

    #add_executable( PandRTest PandRTest.cpp)
    #target_link_libraries(PandRTest catch libfcnpr)
    #add_test(NAME test_pandr COMMAND $<TARGET_FILE:PandRTest>)
    #target_compile_definitions(PandRTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

    add_executable( PlacementAndRoutingTest PlacementAndRoutingTest.cpp)
    target_link_libraries(PlacementAndRoutingTest catch libfcnpr)
    add_test(NAME test_placementandrouting COMMAND $<TARGET_FILE:PlacementAndRoutingTest>)
    target_compile_definitions(PlacementAndRoutingTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")


else()
    message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
