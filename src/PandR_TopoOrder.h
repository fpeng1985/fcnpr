
#pragma once

#include <unordered_map>
#include <map>
#include <boost/container_hash/hash.hpp>

#include "Types.h"

namespace std {
    template<>
    struct hash< std::pair<fcnpr::Node, fcnpr::Node> > {
        using argument_type = std::pair<fcnpr::Node, fcnpr::Node>;
        using result_type   = std::size_t;

        result_type operator()(argument_type const &src_tgt) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, src_tgt.first);
            boost::hash_combine(seed, src_tgt.second);
            return seed;
        }
    };
}

namespace fcnpr {
    
    class PandRTopoOrder {
    public:
        PandRTopoOrder();
        bool run() noexcept;
        void pr_result() noexcept;
    private:
        void init_primary_input_positions() noexcept;
        
        static const uint32_t LEVEL_MULTIPLIER = 4;

        std::vector< std::pair<Node, std::vector<Node>> > traversals;
        std::size_t num_node;
        
        struct PRData {
            std::vector<Position> positions;
            std::vector<Position>::size_type index = -1;
            std::vector<Route> routings;
            bool initialized = false;
            bool placed = false;
            bool routed = false;
            
            Position &position() noexcept {
                return positions[index];
            }
            
            bool exhausted() const noexcept {
                return index == positions.size()-1;
            }
        };
        std::unordered_map<Node, PRData> prs;
    };
    
}
