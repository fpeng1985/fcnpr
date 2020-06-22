//
// Created by fpeng on 2020/6/15.
//

#ifndef ROPPER_LEVELPLACEMENT_H
#define ROPPER_LEVELPLACEMENT_H

#include <set>
#include <unordered_map>
#include <optional>

#include "ChessBoard.h"
#include "Network.h"

namespace fcnpr {

    class Solution;
    constexpr uint32_t LEVEL_MULTIPLIER = 4;

    class LevelPlacement {
    public:
        LevelPlacement(ChessBoard &chb, const Network &ntk, Solution &sln,
                std::unordered_map<Id, std::vector<Position>> cdt);
        LevelPlacement(ChessBoard &chb, const Network &ntk, Solution &sln, Level);
        std::optional<Position> position(Id) const noexcept;
    private:
        ChessBoard &chess_board;
        Network &network;
        Solution &solution;

        std::unordered_map<Id, std::vector<Position>> candidates;
        std::unordered_map<Id, std::vector<Position>::size_type> current_placement;

    private:
        std::vector<Position> candidate_position_for(Id) const;

    };

}



#endif //ROPPER_LEVELPLACEMENT_H
