//
// Created by fpeng on 2020/6/17.
//

#ifndef ROPPER_PANDR_H
#define ROPPER_PANDR_H

#include "ChessBoard.h"
#include "Network.h"
#include "Solution.h"

namespace fcnpr {

    class PandR {
    public:
        PandR(ChessBoard &chb, const Network &ntk);
        bool init_first_level();
        bool place_nth_level(std::size_t n);
        bool backtrack_previous_levels(std::size_t &n);
        bool run();
    private:
        ChessBoard &chess_board;
        Network &network;
        Solution solution;

        uint64_t initial_distance = 0;
    };

}


#endif //ROPPER_PANDR_H
