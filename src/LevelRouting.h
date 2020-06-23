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

        bool route_current_level() noexcept;
        bool unroute_current_level() noexcept;

    private:
        ChessBoard &chess_board;
        Network &network;
        Solution &solution;

        Level level;

        std::map<std::pair<Id, Id>, Route> routings;
    };

}

#endif //ROPPER_LEVELROUTING_H
