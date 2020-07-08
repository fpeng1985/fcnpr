//
// Created by Administrator on 2020/7/7.
//

#ifndef FCNPR_PLACEMENTANDROUTING_H
#define FCNPR_PLACEMENTANDROUTING_H

#include <set>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <cassert>

#include <ChessBoard.h>
#include <Network.h>
#include <Json.h>

namespace fcnpr {

    constexpr uint32_t LEVEL_MULTIPLIER = 4;

    class PlacementAndRouting{

    public:
        PlacementAndRouting();

        struct levelplacement{
            std::unordered_map<Node, std::vector<Position>> candidates;
            std::map<Node, std::vector<Position>::size_type> current_indices;
            Level level;

        };
        struct levelrouting{
            std::map<std::pair<Node, Node>, Route> routes;
            Level level;
        };
        std::vector<levelplacement> placements;
        std::vector<levelrouting> routings;

        inline levelplacement &current_placement() noexcept {
            assert(!placements.empty());
            return placements.back();
        }
        inline levelrouting &current_routing() noexcept {
            assert(!routings.empty());
            return routings.back();
        }
        inline const levelplacement &current_placement() const noexcept {
            assert(!placements.empty());
            return placements.back();
        }
        inline const levelrouting  &current_routing() const noexcept {
            assert(!routings.empty());
            return routings.back();
        }

    private:
        std::optional<Position> position(Node node,levelplacement lp)const noexcept;
        std::map<Node, Position> current_positions(levelplacement lp) const noexcept;

        std::optional<Position> find_position(Node node) const noexcept;
        std::map<Node, Position> find_level_positions(Level level) const noexcept;

        bool find_next_group_of_positions(levelplacement lp) noexcept;

        bool empty(levelplacement lp) const noexcept;
        bool exhausted(levelplacement lp) const noexcept;

        std::vector<Position> candidate_position_for(Node node) const;
        void advance_current_positions(levelplacement lp) noexcept;
        bool place_current_level_of_nodes(levelplacement lp) noexcept;
        void unplace_current_level_of_nodes(levelplacement lp) noexcept;

        bool wire_current_level_of_routes(levelrouting lr) noexcept;
        void unwire_current_level_of_routes(levelrouting lr) noexcept;

        bool init_first_level();
        bool place_nth_level(std::size_t n);
        bool backtrack_previous_levels(std::size_t &n);

        double duration() const noexcept;
        Json json() const noexcept;

    private:
        double pandr_duration{0};

    public:
        bool run();
    };
}
#endif //FCNPR_PLACEMENTANDROUTING_H
