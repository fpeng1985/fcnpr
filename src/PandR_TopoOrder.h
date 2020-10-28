
#ifndef PANDR_TOPO_ORDER_H
#define PANDR_TOPO_ORDER_H

#include <unordered_map>
#include <map>

#include "Types.h"

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

#endif
