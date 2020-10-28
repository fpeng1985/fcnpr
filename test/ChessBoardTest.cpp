//
// Created by Administrator on 2020/6/28.
//
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <ChessBoard.h>
#include <iostream>

using namespace fcnpr;
using namespace Catch;

TEST_CASE("ChessBoard.cpp testing" "[ChessBoard]"){

    ChessBoard& cbd = chessboard();

    Position pos1{1,1};
    Position pos2{5,5};
    Position pos3{10,10};
    Position pos4{10,11};
    Position pos5{10,12};
    Position pos6{10,13};
    Position pos7{20,20};
    Position pos9{2,0};
    Position pos10{2,1};
    Position pos11{2,2};
    Position pos12(3,1);

    Route route1{pos3,pos4,pos5,pos6};

    Node node1{2};
    Node node2{4};
    Node node3{6};

    REQUIRE( cbd.size() == 30 );

    REQUIRE( cbd.cell_at(pos1).empty() );
    REQUIRE( cbd.cell_at(pos3).empty() );

    REQUIRE( cbd.cell_at(pos1).has_node() == false );
    REQUIRE( cbd.place_node(pos1, node1) );
    REQUIRE( cbd.is_empty_in(pos1)  == false);
    REQUIRE( cbd.has_node_in(pos1) );

    REQUIRE( cbd.cell_at(pos2).has_node() == false );
    REQUIRE( cbd.place_node(pos2, node2) );
    REQUIRE( cbd.is_empty_in(pos2) == false );
    REQUIRE( cbd.has_node_in(pos2) );

    REQUIRE(cbd.compute_layout_area() == 25 );

    REQUIRE( cbd.wire_route(route1) );
    REQUIRE( cbd.is_empty_in(pos4) == false);
    REQUIRE( cbd.is_empty_in(pos5) == false);

    REQUIRE( cbd.compute_layout_area() == 120 );
    cbd.unplace_node(pos2);
    REQUIRE( cbd.compute_layout_area() == 120 );

    cbd.unwire_route(route1);
    REQUIRE( cbd.is_empty_in(pos4) );
    REQUIRE( cbd.is_empty_in(pos5) );

    REQUIRE( cbd.compute_layout_area() == 1 );//only pos1 has node

    cbd.place_node(pos7, node3);
    REQUIRE( cbd.compute_layout_area() == 400 );

    cbd.resize(50);
    REQUIRE( cbd.size() == 50 );

    REQUIRE( cbd.time_zone_of(pos1)  == 3 );
    REQUIRE( cbd.time_zone_of(pos9)  == 3 );
    REQUIRE( cbd.time_zone_of(pos10) == 4 );
    REQUIRE( cbd.time_zone_of(pos11) == 1 );
    REQUIRE( cbd.time_zone_of(pos12) == 1 );

    REQUIRE( cbd.exists_datapath_between(pos1,pos10) );
    REQUIRE( cbd.exists_datapath_between(pos9,pos10) );
    REQUIRE( cbd.exists_datapath_between(pos10,pos12) );
    REQUIRE( cbd.exists_datapath_between(pos10,pos11) );
    REQUIRE( cbd.exists_datapath_between(pos12,pos10) == false );

    REQUIRE( cbd.find_route_between(pos1,pos12).has_value());
    REQUIRE( cbd.find_route_between(pos1,pos5).has_value());
    REQUIRE( cbd.find_route_between(pos1,pos11).has_value());
    REQUIRE( cbd.find_route_between(pos3,pos2).has_value());

    auto ngh = cbd.neighbours(pos10,1);
    REQUIRE( ngh.size() == 2 );
    REQUIRE( ( ngh.at(0).first == 2 && ngh.at(0).second ==2 ) );
    REQUIRE( ( ngh.at(1).first == 3 && ngh.at(1).second ==1 ) );

}

