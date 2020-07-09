
#define CATCH_CONFIG_MAIN


#include <catch.hpp>

#define private public

#include "ChessBoard.h"
#include "Network.h"
#include "PandR.h"
#include <iostream>

using namespace fcnpr;
using namespace Catch;

TEST_CASE("PandR.cpp testing" "[c17.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/c17.v";
    ntk.parse(fname);

    auto pandr = PandR();
    if(pandr.run()) {
        pandr.pr_result();
    } else {
        std::cout << "No solution." << std::endl;
    }
}
