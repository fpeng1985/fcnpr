//
// Created by fpeng on 2020/6/23.
//

#ifndef ROPPER_LEVELROUTING_H
#define ROPPER_LEVELROUTING_H

#include "ChessBoard.h"
#include "Network.h"

namespace fcnpr {

    class Solution;

    class LevelRouting {
        LevelRouting(ChessBoard &chb, const Network &ntk, const Solution &sln, Level l);

        bool place_current_wires() noexcept;
        bool unplace_current_wires() noexcept;

    private:
        ChessBoard &chess_board;
        Network &network;
        Solution &solution;

        Level level;
    };

}

#endif //ROPPER_LEVELROUTING_H
