//
// Created by fpeng on 2020/7/7.
//

#define CATCH_CONFIG_MAIN


#include <catch.hpp>

#define private public

#include "ChessBoard.h"
#include "Network.h"
#include "PandR_LevelPR.h"
#include <iostream>

using namespace fcnpr;
using namespace Catch;

TEST_CASE("PandR_LevelPR.cpp testing" "[c17.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/c17.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}

/*
TEST_CASE("PandR_LevelPR.cpp testing" "[xor2.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/xor2.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/
/*
TEST_CASE("PandR_TopoOrder.cpp testing" "[1bitAdderMaj.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/1bitAdderMaj.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/
/*
TEST_CASE("PandR_LevelPR.cpp testing" "[1bitAdderAOIG.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/1bitAdderAOIG.v.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/
/*
TEST_CASE("PandR_LevelPR.cpp testing" "[mux21.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/mux21.v.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/
/*
TEST_CASE("PandR_LevelPR.cpp testing" "[mux41.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/mux41.v.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/
/*
TEST_CASE("PandR_LevelPR.cpp testing" "[clpl.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/clpl.v.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if (pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/
/*
TEST_CASE("PandR_LevelPR.cpp testing" "[xor5R.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/xor5R.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if (pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/

/*TEST_CASE("PandR_LevelPR.cpp testing" "[FA.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/xor5R.v";
    ntk.parse(fname);

    auto pandr = PandRLevelPR();
    if (pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
*/