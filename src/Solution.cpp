//
// Created by fpeng on 2020/6/22.
//

#include "Solution.h"
#include <algorithm>
#include <cassert>

namespace fcnpr {

    std::optional<Position> Solution::find_position(Id) const noexcept {
        auto ret = std::nullopt;

        for(auto it=std::crbegin(placements), it!=std::crend(placements),++it) {
            auto pos = it->position(Id);
            if(pos) {
                ret = pos;
                break;
            }
        }
        return ret;
    }

    std::map<Node, Position> Solution::find_level_positions(Level level) const noexcept {
        assert(level < placements.size());
        return placements[level].current_positions();
    }

    void Solution::push_placement(LevelPlacement &&level_placement) noexcept {
        placements.push_back(std::forward<LevelPlacement>(level_placement));
    }

    void push_routing(LevelRouting &&level_routing) noexcept {
        routings.push_back(std::forward<LevelRouting>(level_routing));
    }

    void Solution::pop_placement() {
        placements.pop_back();
    }

    void Solution::pop_routing() {
        routings.pop_back();
    }

}