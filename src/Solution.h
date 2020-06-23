//
// Created by fpeng on 2020/6/22.
//

#ifndef ROPPER_SOLUTION_H
#define ROPPER_SOLUTION_H

#include <stack>
#include <unordered_map>
#include <optional>

#include "Types.h"
#include "ChessBoard.h"
#include "Network.h"
#include "LevelPlacement.h"

namespace fcnpr {

    class Solution {
    public:
        Solution(ChessBoard &chb, const Network &ntk);
        std::optional<Position> find_position(Id) const noexcept;
        const std::map<Id, std::vector<Position>::size_type> &find_level_positions(Level level) const noexcept;
    private:
        ChessBoard &chess_board;
        Network &network;
        std::vector<LevelPlacement> placements; //保存各层节点的放置
    };

}

#endif //ROPPER_SOLUTION_H
