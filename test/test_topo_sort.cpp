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
    //auto topo_order = ntk.topological_order();
    
    //std::cout << "========================" << std::endl;
    
    auto traversal = ntk.establish_traversal_data();
    
    for(auto &trav: traversal) {
        std::cout << "Node " << trav.first << std::endl;
        std::cout << "fanins ";
        for(auto &fanin : trav.second) {
            std:: cout << fanin << " ";
        }
        std::cout << std::endl;
    }
    
}
