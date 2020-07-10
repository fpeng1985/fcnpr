//
// Created by fpeng on 2020/6/22.
//

#ifndef ROPPER_NETWORK_H
#define ROPPER_NETWORK_H
#include <iostream>
#include <vector>
#include <list>
#include <mockturtle/mockturtle.hpp>
#include "Types.h"

namespace fcnpr {

    using namespace mockturtle;

    class Network {
    public:
        void parse(const std::string &fname);
        std::vector< std::pair<Node, std::vector<Node>> > establish_traversal_data() noexcept;

        uint32_t depth() const;
        uint32_t level_distance(Node const n, Node const m) const;

        std::vector<Node> nodes_at_level(Level const l) const;
        std::vector<Node> fan_ins_of(Node const n) const;
        
        bool is_pi(const Node &node) const {
            return ntk.is_pi(node);
        }

        friend std::ostream& operator<<(std::ostream& ostr, const Network &src);
    private:
        mockturtle::mig_network ntk;
        
        Network() = default;
        friend Network &network();
    };

    Network &network();

}

#endif //ROPPER_NETWORK_H
