//
// Created by fpeng on 2020/6/23.
//

#ifndef ROPPER_LEVELROUTING_H
#define ROPPER_LEVELROUTING_H

#include "ChessBoard.h"
#include "Network.h"

namespace fcnpr {

    class Solution;
    using FoFiRoute = std::tuple<uint64_t,uint64_t,Route const*>;

    class LevelRouting {
        LevelRouting(ChessBoard &chb, const Network &ntk, const Solution &sln, Level l);

        bool place_current_wires() noexcept;
        bool unplace_current_wires() noexcept;

    private:
        ChessBoard &chess_board;
        Network &network;
        Solution &solution;

        Level level;
       // std::map<Level, std::vector<FoFiRoute>>  routes_level;

    };

}

#endif //ROPPER_LEVELROUTING_H
