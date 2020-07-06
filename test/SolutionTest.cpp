//
// Created by Administrator on 2020/7/4.
//

#define CATCH_CONFIG_MAIN


#include <catch.hpp>

#define private public

#include <ChessBoard.h>
#include <LevelPlacement.h>
#include <Solution.h>
#include <Network.h>
#include <LevelRouting.h>
#include <iostream>

using namespace fcnpr;
using namespace Catch;

void parse(const std::string &fname, Network ntk){
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
    solution->push_routing(LevelRouting(solution, 0));
    return true;
}


TEST_CASE("Solution.cpp testing" "[xor2.v]") {
    Network& ntk = network();
    auto solution = std::make_shared<Solution>();

    std::string fname = std::string(TESTCASE) + "/xor2.v";
    parse(fname,ntk);

    REQUIRE(init_first_level(solution, ntk));
    REQUIRE(solution->placements.size() == 1);
    REQUIRE(solution->current_placement().current_indices.size() == 2);
    REQUIRE(solution->routings.empty()   == false);
    REQUIRE(solution->current_routing().routings.empty());

    //level1 and routing1
    LevelPlacement level_placement1(solution, 1);
    while (!level_placement1.exhausted()) {
        if (level_placement1.find_next_group_of_positions()) {
            solution->push_placement(std::move(level_placement1));
            REQUIRE(solution->placements.size() == 2);
            LevelRouting level_routing1(solution, 1);
            if (level_routing1.wire_current_level_of_routes()) {
                solution->push_routing(std::move(level_routing1));
                break;
            }
        }
    }

    std::cout << solution->json() << std::endl;
    REQUIRE(solution->size()            == 2);
    REQUIRE(solution->routings.size()   == 2);
    solution->pop_placement();
    solution->pop_routing();
    REQUIRE(solution->placements.size() == 1);
    REQUIRE(solution->routings.size()   == 1);

}