//
// Created by fpeng on 2020/6/22.
//

#ifndef ROPPER_SOLUTION_H
#define ROPPER_SOLUTION_H

#include <stack>
#include <optional>
#include <cassert>

#include "Types.h"
#include "ChessBoard.h"
#include "Network.h"
#include "LevelPlacement.h"
#include "LevelRouting.h"
#include "Json.h"

namespace fcnpr {

    class Solution {
    public:
        std::optional<Position> find_position(Node) const noexcept;
        std::map<Node, Position> find_level_positions(Level level) const noexcept;

        void push_placement(LevelPlacement &&level_placement) noexcept;
        void push_routing(LevelRouting &&level_routing) noexcept;
        void pop_placement();
        void pop_routing();

        inline std::size_t size() const noexcept {
            return placements.size();
        }

        inline LevelPlacement &current_placement() noexcept {
            assert(!placements.empty());
            return placements.back();
        }
        inline LevelRouting   &current_routing() noexcept {
            assert(!routings.empty());
            return routings.back();
        }
        inline const LevelPlacement &current_placement() const noexcept {
            assert(!placements.empty());
            return placements.back();
        }
        inline const LevelRouting   &current_routing() const noexcept {
            assert(!routings.empty());
            return routings.back();
        }

        Json json() const noexcept;

    private:
        std::vector<LevelPlacement> placements;
        std::vector<LevelRouting>   routings;
    };

}

#endif //ROPPER_SOLUTION_H
