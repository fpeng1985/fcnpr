//
// Created by Administrator on 2020/7/8.
//

#define CATCH_CONFIG_MAIN


#include <catch.hpp>

#define private public

#include <ChessBoard.h>
#include <Network.h>
#include <PlacementAndRouting.h>
#include <iostream>
#include <fstream>

using namespace fcnpr;
using namespace Catch;

void parse(const std::string &fname, Network ntk){
    std::ifstream ifs(fname);
    auto result = lorina::read_verilog( ifs, mockturtle::verilog_reader( ntk.get() ) );
    REQUIRE( result == lorina::return_code::success );
}

TEST_CASE("PandR.cpp testing" "[c17.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/c17.v";
    parse(fname, ntk);

    auto pandr = PlacementAndRouting();
    if(pandr.run()) {
    std::cout << "duration: " << pandr.duration() <<std::endl;
    std::cout << pandr.json() << std::endl;
    } else {
    std::cout << "No solution." << std::endl;
    }
    auto c17_j= pandr.json();
    std::ofstream myfile("c17_j.json");
    myfile << std::setw(4) << c17_j << std::endl;
}