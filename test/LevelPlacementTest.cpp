//
// Created by Administrator on 2020/7/1.
//

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <ChessBoard.h>

#define private public

#include <LevelPlacement.h>
#include <Solution.h>
#include <iostream>
#include <cassert>

using namespace fcnpr;
using namespace Catch;

/*
    //level 0
    LevelPlacement level_placement(solution, candidates);
    REQUIRE( level_placement.empty() == false );
    solution->push_placement(std::move(level_placement));

    //level 1
    LevelPlacement level_placement1(solution, 1);

    REQUIRE_FALSE(level_placement1.empty());
    REQUIRE_FALSE(level_placement1.exhausted());

    auto level1_nodes = ntk.nodes_at_level(1);
    auto pos_of_node1 = level_placement1.position(level1_nodes[0]);
    REQUIRE( pos_of_node1.has_value() );

    auto cur_pos = level_placement1.current_positions();
    REQUIRE_FALSE(cur_pos.empty());

    REQUIRE(level_placement1.find_next_group_of_positions());
    solution->push_placement(std::move(level_placement1));

    //level 2
    LevelPlacement level_placement2(solution, 2);
    REQUIRE_FALSE(level_placement2.empty());
    REQUIRE_FALSE(level_placement2.exhausted());

    auto level2_nodes = ntk.nodes_at_level(2);
    auto pos_of_node2 = level_placement2.position(level2_nodes[1]);
    REQUIRE( pos_of_node2.has_value() );

    cur_pos = level_placement2.current_positions();
    REQUIRE_FALSE(cur_pos.empty());
    REQUIRE(level_placement2.find_next_group_of_positions());
}
*/


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


TEST_CASE("LevelPlacement.cpp testing" "[c17.v]") {
    Network& ntk = network();
    auto solution = std::make_shared<Solution>();

    std::string fname = std::string(TESTCASE) + "/c17.v";
    auto ntk = parse(fname);
    REQUIRE(ntk.depth() == 3);

    if(init_first_level(solution,ntk)){
        REQUIRE(solution->placements.at(0).candidates.size() == 5);

        LevelPlacement level_placement1(solution, 1);
        REQUIRE( level_placement1.current_indices.begin()->second == 0);
        level_placement1.advance_current_positions();
        REQUIRE( level_placement1.current_indices.begin()->second == 1);

    }
}