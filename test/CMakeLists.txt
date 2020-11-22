if(UT)
    enable_testing()
    message(STATUS "${GREEN} * ${RESET}Testing: ${BLUE}Enabled${RESET}")
    include_directories(${CMAKE_SOURCE_DIR}/src)

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

    #add_executable(PandRTopoOrderTest PandR_TopoOrderTest.cpp)
    #target_link_libraries(PandRTopoOrderTest catch fcnpr)
    #target_compile_definitions(PandRTopoOrderTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")
    #add_test(NAME test_pandr_topo_order COMMAND $<TARGET_FILE:PandRTopoOrderTest>)
    
    #add_executable(PandRLevelPRTest PandR_LevelPRTest.cpp)
    #target_link_libraries(PandRLevelPRTest catch fcnpr)
    #target_compile_definitions(PandRLevelPRTest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")
    #add_test(NAME test_pandr_level_pr COMMAND $<TARGET_FILE:PandRLevelPRTest>)

    add_executable(PandRGATest PandR_GATest.cpp)
    target_link_libraries(PandRGATest catch fcnpr)
    target_compile_definitions(PandRGATest PUBLIC "TESTCASE=\"${CMAKE_CURRENT_SOURCE_DIR}/benchmark\"")
    add_test(NAME test_pandr_ga_pr COMMAND $<TARGET_FILE:PandRGATest>)

else()
    message(STATUS "${GREEN} * ${RESET}Testing: ${YELLOW}Disabled${RESET}")
endif()
