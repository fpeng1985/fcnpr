//
// Created by fpeng on 2020/7/9.
//

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <list>
#include <iostream>
#include <algorithm>

#include <boost/graph/topological_sort.hpp>

#include "Network.h"
//#include <mockturtle/mockturtle.hpp>

using namespace Catch;

TEST_CASE("topo sort testing" "[c17.v]") {
    auto &ntk = fcnpr::network();
    ntk.parse(std::string(TESTCASE) + "/c17.v");
    auto topo_order = ntk.topological_order();
    
    std::for_each(topo_order.begin(), topo_order.end(),[](const auto &node){
        std::cout << node << std::endl;
    });
}
