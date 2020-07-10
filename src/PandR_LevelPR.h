//
// Created by fpeng on 2020/7/7.
//

#ifndef FCNPR_PLACEMENTANDROUTING_H
#define FCNPR_PLACEMENTANDROUTING_H

#include <vector>
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

    class PlacementAndRouting {
    public:
        PlacementAndRouting();
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
