//
// Created by fpeng on 2020/6/15.
//

#include "LevelPlacement.h"

#include <algorithm>
#include <cassert>
#include <set>

#include "Network.h"
#include "ChessBoard.h"
#include "Solution.h"

#include <iostream>

namespace fcnpr {

    LevelPlacement::LevelPlacement(std::shared_ptr<Solution> sln, std::unordered_map<Node , std::vector<Position>> cdt) : solution(sln) {
        std::swap(candidates, cdt);
        for(auto &[id,positions] : candidates) {
           current_indices.insert({id,0});
        }
    }

    LevelPlacement::LevelPlacement(std::shared_ptr<Solution> sln, Level level) : solution(sln) {
        assert(level>0);

        auto nodes = network().nodes_at_level(level);
        for(auto node:nodes) {
            current_indices.insert({node, 0});
            candidates.insert({node, candidate_position_for(node)});
        }
    }

    std::optional<Position> LevelPlacement::position(Node node) const noexcept {
        auto it = current_indices.find(node);
        if (it == current_indices.end())
            return std::nullopt;
        else {
            auto idx = it->second;
            return candidates.at(node).at(idx);
        }
    }

    std::map<Node, Position> LevelPlacement::current_positions() const noexcept {
        std::map<Node, Position> positions;
        for(auto &[node, idx] : current_indices) {
            positions[node] = candidates.at(node).at(idx);
        }
        return positions;
    }


    bool LevelPlacement::find_next_group_of_positions() noexcept {
        assert(!empty());

        int k = 0;
        while(!exhausted()) {
            advance_current_positions();
            if(place_current_level_of_nodes())
                return true;
        }

        return false;
    }

    bool LevelPlacement::empty() const noexcept {
        bool ret = false;
        for(auto &[id, cdt] : candidates) {
            if(cdt.empty()) ret = true;
        }
        return ret;
    }

    bool LevelPlacement::exhausted() const noexcept {
        bool exhausted = true;
        for(auto &[node, pos] : current_indices) {
            exhausted = exhausted && (candidates.at(node).size() == (pos+1));
        }
        return exhausted;
    }

    bool LevelPlacement::place_current_level_of_nodes() noexcept {
        std::vector<Position> placed_positions;
        bool roll_back = false;

        for(auto&[node, index] : current_indices) {
            if( chessboard().place_node(candidates[node][index], node) ) {
                placed_positions.push_back(candidates[node][index]);
            } else {
                roll_back = true;
                break;
            }
        }

        if(roll_back) {
            for(auto &pos : placed_positions) {
                chessboard().unplace_node(pos);
            }
            return false;
        }

        return true;
    }

    void LevelPlacement::unplace_current_level_of_nodes() noexcept {
        for(auto &[node,index] : current_indices) {
            auto pos = candidates[node][index];
            chessboard().unplace_node(pos);
        }
    }

    std::vector<Position> LevelPlacement::candidate_position_for(Node node) const {
        auto size_xy = chessboard().size();
        std::map<Position,std::size_t> position_occurrence;

        auto fanins = network().fan_ins_of(node);
        Position fanin_pos;

        for( const auto&fanin : fanins ) {
            assert(solution->find_position(fanin).has_value());
            if(solution->find_position(fanin).has_value()){
                fanin_pos = solution->find_position(fanin).value();
            }
            auto &x = fanin_pos.first;
            auto &y = fanin_pos.second;
            auto distance = network().level_distance(node, fanin) * LEVEL_MULTIPLIER;

            uint64_t floor_i = (x-distance >= 0)? x-distance : 0;
            uint64_t floor_j = (y-distance >= 0)? y-distance : 0;
            uint64_t top_i = (x+distance < size_xy-1)? x+distance : size_xy-1;
            uint64_t top_j = (y+distance < size_xy-1)? y+distance : size_xy-1;

            for(auto i=floor_i; i<=top_i; i++){
                for(auto j=floor_j; j<=top_j; j++){
                    Position cur_pos = {i,j};
                    auto route = chessboard().find_route_between(fanin_pos, cur_pos);

                    if(route.has_value() && route.value().size() == (distance+1)) {
                        position_occurrence[cur_pos]++;
                    }
                }
            }
        }

        std::vector<Position> positions;
        for(auto &[pos, occurrences] : position_occurrence) {
            if(occurrences == fanins.size()) {
                positions.push_back(pos);
            }
        }

        return positions;
    }

    void LevelPlacement::advance_current_positions() noexcept {
        assert(!exhausted());

        std::vector<Position>::size_type sum = 0;
        for(auto &[id, pos] : current_indices) {
            sum += pos;
        }

        //if(sum!=0) {
            for(auto &[id, pos] : current_indices) {
                pos = (pos+1) % candidates[id].size();
                if(pos==0) continue;
                else break;
            }
        //}
    }
}
