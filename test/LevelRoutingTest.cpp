//
// Created by Administrator on 2020/7/3.
//

#define CATCH_CONFIG_MAIN


#include <catch.hpp>

#define private public

#include <ChessBoard.h>
#include <LevelPlacement.h>
#include <Solution.h>
#include <iostream>
#include <fstream>

using namespace fcnpr;
using namespace Catch;

void parse(const std::string &fname,Network& ntk){
    std::ifstream ifs(fname);
    auto result = lorina::read_verilog( ifs, mockturtle::verilog_reader( ntk.get() ) );
    REQUIRE( result == lorina::return_code::success );
}

bool init_first_level(std::shared_ptr<Solution>& solution, const Network& ntk){
    auto initial_nodes = ntk.nodes_at_level(0);
    auto middle = chessboard().size() / 2;

    uint64_t initial_distance = 1;
    std::vector<Position> initial_positions;
    while(true) {
        initial_positions = chessboard().neighbours({middle, middle}, initial_distance);
        if(initial_nodes.size() <= initial_positions.size()) break;
        else ++initial_distance;
    }

    std::unordered_map<Node, std::vector<Position>> candidates;
    for(auto i=0; i<initial_nodes.size(); ++i) {
        candidates[initial_nodes[i]] = initial_positions;
    }

    LevelPlacement level_placement0(solution, candidates);
    REQUIRE(level_placement0.empty() == false);
    while(!level_placement0.find_next_group_of_positions()) {
        if(level_placement0.exhausted())
            return false;
    }
    solution->push_placement(std::move(level_placement0));
    return true;
}


TEST_CASE("LevelRouting.cpp testing""[c17.v]") {
    Network& ntk = network();
    auto solution = std::make_shared<Solution>();

    std::string fname = std::string(TESTCASE) + "/c17.v";
    parse(fname,ntk);
    REQUIRE(ntk.depth() == 3);

    REQUIRE(init_first_level(solution, ntk));
    LevelPlacement level_placement1(solution, 1);
    while(!level_placement1.find_next_group_of_positions()) {
        if(level_placement1.exhausted())
            REQUIRE( level_placement1.exhausted() == false);
    }
    solution->push_placement(std::move(level_placement1));

    //routing1
    LevelRouting level_routing1(solution, 1);

    auto lep1 = solution->placements.at(1);
    while (!lep1.exhausted()) {
        if (lep1.find_next_group_of_positions()) {
            if (level_routing1.wire_current_level_of_routes()) {
                REQUIRE(level_routing1.routings.size() == 4);
                REQUIRE(level_routing1.begin() == level_routing1.routings.begin());
                REQUIRE(level_routing1.end() == level_routing1.routings.end());
                break;
            }
        }
    }
    level_routing1.unwire_current_level_of_routes();
    REQUIRE(level_routing1.routings.empty());
}

/*TEST_CASE("LevelRouting.cpp testing""[xor2.v]"){
    Network& ntk = network();
    auto solution = std::make_shared<Solution>();

    std::string fname = std::string(TESTCASE) + "/xor2.v";
    parse(fname,ntk);

    REQUIRE(init_first_level(solution, ntk));
    LevelPlacement level_placement1(solution, 1);
    while(!level_placement1.find_next_group_of_positions()) {
        if(level_placement1.exhausted())
            REQUIRE( level_placement1.exhausted() == false);
    }
    solution->push_placement(std::move(level_placement1));

    //routing1
    LevelRouting level_routing1(solution, 1);

    auto lep1 = solution->placements.at(1);
    while (!lep1.exhausted()) {
        if (lep1.find_next_group_of_positions()) {
            if (level_routing1.wire_current_level_of_routes()) {
                REQUIRE(level_routing1.routings.empty() == false);
                REQUIRE(level_routing1.begin() == level_routing1.routings.begin());
                REQUIRE(level_routing1.end() == level_routing1.routings.end());
                break;
            }
        }
    }
    level_routing1.unwire_current_level_of_routes();
    REQUIRE(level_routing1.routings.empty());
}
*/
/*TEST_CASE("LevelRouting.cpp testing""[mux21.v]"){
    auto solution = std::make_shared<Solution>();
    Network& ntk = network();

    std::string fname = std::string(TESTCASE) + "/mux21.v";
    parse(fname ,ntk);

    REQUIRE(init_first_level(solution, ntk));
    LevelPlacement level_placement1(solution, 1);
    REQUIRE(level_placement1.find_next_group_of_positions());
    while(!level_placement1.find_next_group_of_positions()) {
        if(level_placement1.exhausted())
            REQUIRE( level_placement1.exhausted() == false);
    }
    solution->push_placement(std::move(level_placement1));

    //routing1
    LevelRouting level_routing1(solution, 1);

    auto lep1 = solution->placements.at(1);
    while (!lep1.exhausted()) {
        if (lep1.find_next_group_of_positions()) {
            if (level_routing1.wire_current_level_of_routes()) {
                REQUIRE(level_routing1.routings.empty() == false);
                REQUIRE(level_routing1.begin() == level_routing1.routings.begin());
                REQUIRE(level_routing1.end() == level_routing1.routings.end());
                break;
            }
        }
    }
    level_routing1.unwire_current_level_of_routes();
    REQUIRE(level_routing1.routings.empty());
}

*/