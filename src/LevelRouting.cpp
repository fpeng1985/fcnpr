//
// Created by fpeng on 2020/6/23.
//

#include "LevelRouting.h"

namespace fcnpr {

    LevelRouting::LevelRouting(ChessBoard &chb, const Network &ntk, const Solution &sln, Level l) :
            chess_board(chb), network(ntk), solution(sln), level(l) {
    }

    bool LevelRouting::route_current_level() noexcept {
        assert(level>0);

        auto nodes = network.nodes_at_level(level);
        auto current_level_positions  = solution.find_level_positions(level);
        std::vector<Route> routed_wires;

        for(auto &node : nodes) {
            auto fanins = network.node_fan_ins(node);

            for(auto &fanin : fanins) {
                auto cur_pos = current_level_positions[node];
                auto fanin_pos = solution.find_position(fanin).value();
                auto path = chess_board.find_path_between(cur_pos, fanin_pos);

                if( chess_board.place_wire(path) ) {
                    routed_wires.push_back(path);
                    continue;
                }
                else {
                    for(auto &wire: routed_wires) {
                        chess_board.unplace_wire(wire);
                    }
                    return false;
                }
            }
        }

        return true;
    }

    bool LevelRouting::unroute_current_level() noexcept {
        assert(level>0);

        auto nodes = network.nodes_at_level(level);
        auto current_level_positions  = solution.find_level_positions(level);

        for(auto &node : nodes) {
            auto fanins = network.node_fan_ins(node);

            for(auto &fanin : fanins) {
                auto cur_pos = current_level_positions[node];
                auto fanin_pos = solution.find_position(fanin).value();
                auto wire = chess_board.find_path_between(cur_pos, fanin_pos);

                chess_board.unplace_wire(wire);
            }
        }

        return true;
    }

}