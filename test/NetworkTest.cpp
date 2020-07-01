//
// Created by Administrator on 2020/7/1.
//

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Network.h>
#include <iostream>

using namespace fcnpr;
using namespace Catch;

TEST_CASE("Network.cpp testing" "NetworkTest"){

    Network& ntk = network();

    std::string file{
            "module C17.iscas  (pi0, pi1, pi2, pi3, pi4,po0, po1  );\n"
            "input  pi0, pi1, pi2, pi3, pi4;\n"
            "output po0, po1;\n"
            "wire n9, n10, n11, n13;\n"
            "assign n9 = pi0 & pi2;\n"
            "assign n10 = pi2 & pi3;\n"
            "assign n11 = ~n10 & pi1;\n"
            "assign po0 = n11 | n9;\n"
            "assign n13 = ~n10 & pi4;\n"
            "assign po1 = n11 | n13;\n"
            "endmodule\n"};

    std::istringstream in( file );
    auto result = lorina::read_verilog( in, mockturtle::verilog_reader( ntk.get() ) );

    REQUIRE( result == lorina::return_code::success );
    REQUIRE( ntk.depth() == 3 );

    auto nodes_of_level0 = ntk.nodes_at_level(0);
    auto nodes_of_level1 = ntk.nodes_at_level(1);
    auto nodes_of_level3 = ntk.nodes_at_level(3);
    REQUIRE( nodes_of_level0.size() == 5 );
    REQUIRE( nodes_of_level1.size() == 2 );
    REQUIRE( nodes_of_level3.size() == 2 );

    REQUIRE( ntk.level_distance(nodes_of_level1[0],nodes_of_level3[0]) == 2);

    auto finins_of_node1 = ntk.fan_ins_of(nodes_of_level1[0]);
    auto finins_of_node6 = ntk.fan_ins_of(nodes_of_level3[1]);
    REQUIRE(finins_of_node1.size() == 2 );
    REQUIRE(finins_of_node6.size() == 2 );

    std::cout << "ntk: " << ntk << std::endl;

}

/*
    Network& ntk = network();
    Network& parse(const std::string &fname,Network& ntk){
        std::istringstream in( fname ) ;
        auto result = lorina::read_verilog( in, mockturtle::verilog_reader( ntk.get() ) );
        REQUIRE( result == lorina::return_code::success );
        return ntk;
    }


    TEST_CASE("c17.v ntk"){
        std::string fname  = std::string(TESTCASE) + "/c17.v";
        auto c17ntk = parse(fname,ntk);
    }
     */
