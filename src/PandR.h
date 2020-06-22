//
// Created by fpeng on 2020/6/17.
//

#ifndef ROPPER_PANDR_H
#define ROPPER_PANDR_H

#include <set>
#include "ChessBoard.h"

namespace fcnpr {

    using Placements = std::set<Placement>;
    using PlacementCandidate = std::map<Id, std::set<Position>>;

    class PandR {
    public:
        PandR(ChessBoard &chssbd, const Network &ntwk);
        void build_placement_candidates();
        bool place_first_level();
        bool place_nth_level(std::size_t n);
    private:
        ChessBoard &chess_board;
        std::unordered_map<std::size_t, std::set<Id>> level_of_node;

        PlacementCandidate placement_candidates;

        Placements first_level_placement;
        std::unordered_map<std::size_t, Placements> placement_result;
        std::unordered_map<Placements,std::vector<Placements> placement_blacklist;



    };

}


#endif //ROPPER_PANDR_H
