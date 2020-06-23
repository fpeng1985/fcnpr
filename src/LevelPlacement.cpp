//
// Created by fpeng on 2020/6/15.
//

#include "LevelPlacement.h"
#include <algorithm>
#include <cassert>
#include <set>

namespace fcnpr {

    LevelPlacement::LevelPlacement(ChessBoard &chb, const Network &ntk, Solution &sln,
            std::unordered_map<Id, std::vector<Position>> cdt) :
            chess_board(chb), network(ntk), solution(sln) {
        std::swap(candidates, cdt);
        for(auto &[id,positions] : candidates) {
           current_placement.insert({id,0});
        }
    }


    LevelPlacement::LevelPlacement(ChessBoard &chb, const Network &ntk, Solution &sln, Level level) :
            chess_board(chb), network(ntk), solution(sln) {
        assert(level>0);

        auto nodes = network.nodes_at_level(level);

        for(auto node:nodes) {
            current_placement.insert({node, 0})
            candidates.insert({node, candidate_position_for(node)});
        }

        //TODO:
    }

    std::optional<Position> LevelPlacement::position(Id) const noexcept {
        auto it = current_placment.find(Id);
        if (it == current_placement.end())
            return std::nullopt;
        else {
            auto idx = it->second;
            return candidates[Id][idx];
        }
    }

    const std::map<Id, std::vector<Position>::size_type> &LevelPlacement::current_positions() const noexcept {
        return current_placement;
    }


    std::optional<std::map<Id,Position>> LevelPlacement::find_next_group_of_positions() const {
        assert(!empty());

        while(!exhausted()) {
            advance_current_placement();
            if(place_current_positions())
                return current_placement;
        }

        return std::nullopt;
    }

    bool LevelPlacement::empty() const noexcept {
        bool ret = false;
        for(auto &[id, cdt] : candadites) {
            if(cdt.empty()) ret = true;
        }
        return ret;
    }

    bool LevelPlacement::exhausted() const noexcept {
        bool exhausted = true;
        for(auto &[id, pos] : current_placement) {
            exhausted = exhausted && (candidates[id].size() == (pos+1))
        }
        return exhausted;
    }

    bool LevelPlacement::place_current_positions() noexcept {
        std::vector<Position> placed_positions;
        bool roll_back = false;

        for(auto&[id, idx] : current_placement) {
            auto pos = candidates[id][idx];
            if( chess_board.place_node(pos, id) ) {
                placed_positions.push_back(pos);
            } else {
                roll_back = true;
                break;
            }
        }

        if(roll_back) {
            for(auto &pos : placed_positions) {
                chess_board.unplace_node(pos);
            }
            return false;
        }

        return true;
    }

    bool LevelPlacement::unplace_current_positions() noexcept {
        for(auto &[id,idx] : current_placement) {
            auto pos = candidates[id][idx];
            chess_board.unplace_node(pos);
        }
        return true;
    }

    std::vector<Position> LevelPlacment::candidate_position_for(Id) const {
        auto size_xy = chess_board.size();
        std::map<Position,std::size_t> position_occurrence;

        auto fanins = network.node_fan_ins(Id);
        for( const auto&fanin : network.node_fan_ins(Id) ) {
            auto fanin_pos = solution.find_position(fanin);
            auto &x = fanin_pos->first;
            auto &y = fanin_pos->second;
            auto distance = network.level_distance(node, fanin) * LEVEL_MULTIPLIER;

            uint64_t floor_i = (x-distance >= 0)? x-distance : 0;
            uint64_t floor_j = (y-distance >= 0)? y-distance : 0;
            uint64_t top_i = (x+distance < size_xy-1)? x+distance : size_xy-1;
            uint64_t top_j = (y+distance < size_xy-1)? y+distance : size_xy-1;

            for(auto i=floor_i; i<=top_i; i++){
                for(auto j=floor_j; j<=top_j; j++){
                    auto cur_pos = {i,j};
                    if(chess_board.find_path_between(fanins_pos, cur_pos) == (distance + 1)){
                        position_occurrence[cur_pos]++;
                    }
                }
            }
        }

        std::vector<Position> positions;
        for(auto &[pos, occurrences] : position_occurrence) {
            if(occurrences.size() == fanins.size()) {
                positions.push_back(pos);
            }
        }

        return positions;
    }

    void Level::advance_current_placement() noexcept {
        assert(!exhausted());

        std::vector<Position>::size_type sum = 0;
        for(auto &[id, pos] : current_placement) {
            sum += pos;
        }

        if(sum!=0) {
            for(auto &[id, pos] : current_placement) {
                pos = (pos+1) % candadites[id].size();
                if(pos==0) continue;
                else break;
            }
        }
    }
}
