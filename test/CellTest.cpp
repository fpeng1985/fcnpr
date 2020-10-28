//
// Created by Administrator on 2020/6/27.
//
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Cell.h>

using namespace fcnpr;
using namespace Catch;

TEST_CASE("Cell.cpp testing" "[Cell]"){
    Cell cell;
    Node node1{2};
    REQUIRE(MAX_CELL_WEIGHT == 6);
    REQUIRE(WIRE_WEIGHT     == 2);
    REQUIRE(NODE_WEIGHT     == 4);


    SECTION("node testing"){
        REQUIRE(cell.empty());
        REQUIRE(cell.put_node(node1));
        REQUIRE(cell.has_node());
        REQUIRE(cell.put_node(node1) == false);
        cell.unput_node();
        REQUIRE(cell.has_node() == false);
    }

    SECTION("wire testing"){
        REQUIRE(cell.put_wire());
        REQUIRE_FALSE(cell.empty());
        REQUIRE(cell.put_wire());
        REQUIRE(cell.put_wire());
        REQUIRE(cell.put_wire() == false);
        cell.unput_wire();
        cell.unput_wire();
        cell.unput_wire();
        REQUIRE(cell.empty());
    }

    SECTION("node and wire testing"){
        REQUIRE(cell.empty());
        REQUIRE(cell.put_node(node1));
        REQUIRE(cell.has_node());
        REQUIRE(cell.put_wire());
        REQUIRE(cell.put_node(node1) == false);
        REQUIRE(cell.put_wire()      == false);
    }
}



