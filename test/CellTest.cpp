//
// Created by Administrator on 2020/6/27.
//

#include"catch2/catch.hpp"
#include "../src/Cell.h"

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
        REQUIRE_FALSE(cell.put_node(node1));
        cell.unput_node();
        REQUIRE_FALSE(cell.has_node());
    }

    SECTION("wire testing"){
        REQUIRE(cell.put_wire());
        REQUIRE_FALSE(cell.empty());
        REQUIRE(cell.put_wire());
        REQUIRE(cell.put_wire());
        REQUIRE_FALSE(cell.put_wire());
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
        REQUIRE_FALSE(cell.put_node(node1));
        REQUIRE_FALSE(cell.put_wire());
    }
}



