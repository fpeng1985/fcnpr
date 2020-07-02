//
// Created by fpeng on 2020/6/22.
//

#include "Solution.h"
#include <algorithm>
#include <cassert>
#include <string>

namespace fcnpr {

    std::optional<Position> Solution::find_position(Node node) const noexcept {
        std::optional<Position> ret = std::nullopt;

        for(auto it=std::crbegin(placements); it!=std::crend(placements); ++it) {
            if (it->position(node).has_value()) {
                auto pos = it->position(node).value();
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

    void Solution::push_routing(LevelRouting &&level_routing) noexcept {
        routings.push_back(std::forward<LevelRouting>(level_routing));
    }

    void Solution::pop_placement() {
        placements.pop_back();
    }

    void Solution::pop_routing() {
        routings.pop_back();
    }

    Json Solution::json() const noexcept {
        Json j;

        j["pandr"]["area"] = chessboard().compute_layout_area();

        auto node_positions = find_level_positions(0);
        for(auto const& [node,pos] : node_positions) {
            j["pandr"]["0"][std::to_string(node)] = pos;
        }

        for(std::size_t level=1; level<routings.size(); ++level) {
            std::string level_str {std::to_string(level)};

            for(auto const &[src_tgt, route] : routings.at(level)) {
                std::string fo_str {std::to_string(src_tgt.first)};
                std::string fi_str {std::to_string(src_tgt.second)};
                j["pandr"][level_str.c_str()][(fi_str + " -> "+fi_str).c_str()]["route"] = route;
            }
        }

        return j;
    }

}