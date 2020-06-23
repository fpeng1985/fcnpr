//
// Created by fpeng on 2020/6/23.
//

#include "LevelRouting.h"

namespace fcnpr {

    LevelRouting::LevelRouting(ChessBoard &chb, const Network &ntk, const Solution &sln, Level l) :
            chess_board(chb), network(ntk), solution(sln), level(l) {
    }

    bool LevelRouting::place_current_wires() noexcept {

    }

    bool LevelRouting::unplace_current_wires() noexcept {

    }

}