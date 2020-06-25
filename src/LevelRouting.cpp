//
// Created by fpeng on 2020/6/23.
//

#include "LevelRouting.h"
#include "Network.h"
#include "ChessBoard.h"
#include "Solution.h"

namespace fcnpr {

    LevelRouting::LevelRouting(std::shared_ptr<Solution> sln, Level l) : solution(sln), level(l)
    {}

    bool LevelRouting::wire_current_level_of_routes() noexcept {
        assert(level>0);

        auto nodes = network().nodes_at_level(level);
        auto current_level_positions  = solution->find_level_positions(level);

        for(auto &node : nodes) {
            auto fanins = network().fan_ins_of(node);

            for(auto &fanin : fanins) {
                Position cur_pos = current_level_positions[node];
                Position fanin_pos = solution->find_position(fanin).value();
                auto route = chessboard().find_route_between(cur_pos, fanin_pos).value();

                if( chessboard().wire_route(route) ) {
                    routings[{node, fanin}] = route;
                    continue;
                }
                else {
                    for(auto &[src_tgt, route]: routings) {
                        chessboard().unwire_route(route);
                    }
                    routings.clear();
                    return false;
                }
            }
        }

        return true;
    }

    void LevelRouting::unwire_current_level_of_routes() noexcept {
        assert(level>0);

        for(auto &[src_tgt, route] : routings) {
            chessboard().unwire_route(route);
        }
        routings.clear();
    }

}