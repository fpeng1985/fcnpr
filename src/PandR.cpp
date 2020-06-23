//
// Created by fpeng on 2020/6/17.
//

#include "PandR.h"
#include "LevelPlacement.h"
#include "LevelRouting.h"

namespace fcnpr {

    PandR::PandR(ChessBoard &chb, const int &ntk): chess_board(chb), network(ntk), solution(chb, ntk)
    {}

    bool PandR::init_first_level() {
        auto initial_nodes = network.nodes_at_level(0);
        auto middle = chess_board.size() / 2;

        initial_distance = 1;
        Region initial_positions;
        while(true) {
            initial_positions = chess_board.neighbours(middle, middle, initial_distance);
            if(initial_nodes.size() <= initial_positions.size()) break;
            else ++initial_distance;
        }

        std::unordered_map<Id, std::vector<Position>> candidates;
        for(auto i=0; i<initial_nodes.size(); ++i) {
            candidates.insert({initial_nodes[i], initial_positions});
        }

        LevelPlacement level_placement(chess_board, network, solution, candidates);
        while(!level_placement.find_next_group_of_positions()) {
            if(level_placement.exhausted())
                return false;
        }
        solution.push_placement(std::move(level_placement));
        solution.push_routing(std::vector<LevelRouting>())
        return true;
    }

    bool PandR::place_nth_level(std::size_t n) {
        LevelPlacement level_placement(chess_board, network, solution, n);

        if(level_placement.empty()) return false;

        while (!level_placement.exhausted()) {
            if(level_placement.find_next_group_of_positions()) {
                LevelRouting level_routing(chess_board, network, solution, n);
                if(level_routing.route_current_level()) {
                    solution.push_placement(std::move(level_placement));
                    solution.push_routing(std::move(level_routing));
                    return true;
                }
            }
        }
        return false;
    }

    bool PandR::backtrack_previous_levels(std::size_t &n) {
        assert(solution.size()-1 == n);

        bool placed = false;
        bool routed = false;
        while(n>=0) {
            solution.current_routing().unroute_current_level();
            solution.current_placement().unplace_current_positions();

            placed = false;
            routed = false;
            if(solution.current_placement().find_next_group_of_positions()) {
                placed = true;
                if(solution.current_routing().route_current_level()) {
                    routed = true;
                }
            }

            if(placed && routed) {
                return true;
            } else {
                if(routed)
                    solution.current_routing().unroute_current_level();
                if(placed)
                    solution.current_placement().unplace_current_positions();
                solution.pop_routing();
                solution.pop_placement();
                n--;
            }
        }
        return false;
    }


    bool PandR::run() {
        init_first_level();

        auto depth = network.depth();
        std::size_t n = 0;

        while(solution.size() != depth) {
            n++;
            if(!place_nth_level(n)) {
                n--;
                if(backtrack_previous_levels(n)) {
                    return false;
                }
            }
        }
        return true;
    }

}