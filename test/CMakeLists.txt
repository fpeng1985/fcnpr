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

    #add_executable( PlacementAndRoutingTest PlacementAndRoutingTest.cpp)
    #target_link_libraries(PlacementAndRoutingTest catch libfcnpr)
    #add_test(NAME test_pandr COMMAND $<TARGET_FILE:PlacementAndRoutingTest>)
    #target_compile_definitions(PlacementAndRoutingTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

    add_executable(TopoSortTest test_topo_sort.cpp)
    target_link_libraries(TopoSortTest catch libfcnpr)
    add_test(NAME test_topo_sort COMMAND $<TARGET_FILE:TopoSortTest>)
    target_compile_definitions(TopoSortTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")
    
    add_executable( PandRTest PandRTest.cpp)
    target_link_libraries(PandRTest catch libfcnpr)
    add_test(NAME test_pandr COMMAND $<TARGET_FILE:PandRTest>)
    target_compile_definitions(PandRTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")

else()
    message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
