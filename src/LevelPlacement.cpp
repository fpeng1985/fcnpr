//
// Created by fpeng on 2020/6/15.
//

#include "LevelPlacement.h"
#include <algorithm>
#include <cassert>

namespace fcnpr {

    LevelPlacement::LevelPlacement(ChessBoard &chb, const Network &ntk, Solution &sln,
            std::unordered_map<Id, std::vector<Position>> cdt) :
            chess_board(chb), network(ntk), solution(sln) {
        std::swap(candidates, cdt);
        for(auto &[id,positions] : candidates) {
           current_placement.insert({id,-1});
        }
    }


    LevelPlacement::LevelPlacement(ChessBoard &chb, const Network &ntk, Solution &sln, Level level) :
            chess_board(chb), network(ntk), solution(sln) {
        assert(level>0);

        auto nodes = network.nodes_at_level(level);

        for(auto node:nodes) {
            current_placement.insert({node, -1})
            candidates.insert({node, candidate_position_for(node)});
        }
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

}
