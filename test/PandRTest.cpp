//
// Created by Administrator on 2020/7/5.
//

#define CATCH_CONFIG_MAIN


#include <catch.hpp>

#define private public

#include <ChessBoard.h>
#include <LevelPlacement.h>
#include <Solution.h>
#include <Network.h>
#include <LevelRouting.h>
#include <PandR.h>
#include <iostream>

using namespace fcnpr;
using namespace Catch;

void parse(const std::string &fname, Network ntk){
    std::ifstream ifs(fname);
    auto result = lorina::read_verilog( ifs, mockturtle::verilog_reader( ntk.get() ) );
    REQUIRE( result == lorina::return_code::success );
}

/*
    TEST_CASE("PandR.cpp testing" "[xor2.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/xor2.v";
    parse(fname, ntk);

    auto pandr = PandR();
    REQUIRE(pandr.init_first_level());

    std::size_t n = 1;
    REQUIRE(pandr.place_nth_level(n));
    REQUIRE(pandr.solution->placements.size() == 2);
    REQUIRE(pandr.solution->routings.size() == 2);
    n++;
    REQUIRE(pandr.place_nth_level(n));
    REQUIRE(pandr.solution->placements.size() == 3);
    REQUIRE(pandr.solution->routings.size() == 3);

    if (!pandr.backtrack_previous_levels(n)) {
        REQUIRE(n == 1);
    }

    REQUIRE(pandr.run());
    std::cout << "duration: " << pandr.duration() <<std::endl;
}
*/

TEST_CASE("PandR.cpp testing" "[c17.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/c17.v";
    parse(fname, ntk);

    auto pandr = PandR();
    if(pandr.run()) {
        std::cout << "duration: " << pandr.duration() <<std::endl;
        std::cout << pandr.solution->json() << std::endl;
    } else {
        std::cout << "No solution." << std::endl;
    }
}

TEST_CASE("PandR.cpp testing" "[mux21.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/mux21.v";
    parse(fname, ntk);

    auto pandr = PandR();
    REQUIRE(pandr.run());
    std::cout << "duration: " << pandr.duration() <<std::endl;
    std::cout << pandr.solution->json() << std::endl;
}

TEST_CASE("PandR.cpp testing" "[1bitAdderMaj.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/1bitAdderMaj.v";
    parse(fname, ntk);

    auto pandr = PandR();
    REQUIRE(pandr.run());
    std::cout << "duration: " << pandr.duration() <<std::endl;
    std::cout << pandr.solution->json() << std::endl;
}

TEST_CASE("PandR.cpp testing" "[FA.v]") {
    Network &ntk = network();
    std::string fname = std::string(TESTCASE) + "/FA.v";
    parse(fname, ntk);

    auto pandr = PandR();
    REQUIRE(pandr.run());
    std::cout << "duration: " << pandr.duration() <<std::endl;
    std::cout << pandr.solution->json() << std::endl;
}
