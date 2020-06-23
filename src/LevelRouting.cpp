//
// Created by fpeng on 2020/6/23.
//

#include "LevelRouting.h"

namespace fcnpr {

    LevelRouting::LevelRouting(ChessBoard &chb, const Network &ntk, const Solution &sln, Level l) :
            chess_board(chb), network(ntk), solution(sln), level(l) {
    }

    bool LevelRouting::place_current_wires() noexcept {
        std::vector<Route> routes_cache;
        if(level <= 0) return;
        auto ps = find_level_positions(level)->current_placement;
        for(auto &[id, idx] : ps ){
            auto const fo_id  = id;
            auto const fo_pos = ps->candidates[fo_id].at(idx);
            auto fanins = ntk.node_fan_ins(fo_id)};
            for(const auto& fanin : network.node_fan_ins(fo_id)){
                auto const fanin_pos = placements.find_position(fanin);
                auto const& curr_path {chess_board.find_path_between(finin_pos,fo_pos)};
                if(chess_board.place_wire(curr_path)){
                    routes_cache.push_back(curr_path);
                    return true;
                }
                return false;
            }
        }
       // routes_level[level] = routes_cache;

    }

    bool LevelRouting::unplace_current_wires() noexcept  {
        if(level <= 0) return false;
        auto ps = find_level_positions(level)->current_placement;
        for(auto &[id, idx] : ps ){
            auto const fo_id  = id;
            auto const fo_pos = ps->candidates[fo_id].at(idx);
            auto fanins = ntk.node_fan_ins(fo_id)};
            for(const auto& fanin : network.node_fan_ins(fo_id)){
                auto const fanin_pos = placements.find_position(fanin);
                auto const& curr_path {chess_board.find_path_between(finin_pos,fo_pos)};
                if(chess_board.unplace_wire(curr_path)){
                    return true;
                }
                return false;
        }
    }
}