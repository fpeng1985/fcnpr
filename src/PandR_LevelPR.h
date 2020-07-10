//
// Created by fpeng on 2020/7/7.
//

#ifndef FCNPR_PLACEMENTANDROUTING_H
#define FCNPR_PLACEMENTANDROUTING_H

#include <vector>
#include <unordered_map>
#include <map>

#include "Types.h"

namespace fcnpr {

    class PandRLevelPR {
    public:
        PandRLevelPR();
        bool run();
        void pr_result() noexcept;
    protected:
        void initialize_level(std::size_t level);
        bool place_nodes_of_level(std::size_t level) noexcept;
        bool route_nodes_of_level(std::size_t level) noexcept;

        Position find_fanin_position_for(const Node &fanin) const noexcept;

        bool level_is_empty(std::size_t level) const noexcept;
        bool level_is_exhausted(std::size_t level) const noexcept;

        void advance_level_indices(std::size_t level) noexcept;

    private:
        static const uint32_t LEVEL_MULTIPLIER = 4;

        std::size_t num_level;
        std::vector<bool> level_has_initialized;
        std::vector<bool> level_has_placed;
        std::vector<bool> level_has_routed;

        std::vector<std::unordered_map<Node, std::vector<Position>>>  candidate_positions;
        std::vector<std::map<Node, std::vector<Position>::size_type>> level_placement_indices;
        std::vector<std::unordered_map<std::pair<Node, Node>, Route>> level_routings;
    };

}


#endif //FCNPR_PLACEMENTANDROUTING_H
