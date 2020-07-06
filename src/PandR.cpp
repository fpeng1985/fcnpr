//
// Created by fpeng on 2020/6/17.
//

#include <unordered_map>

#include "PandR.h"
#include "LevelPlacement.h"
#include "LevelRouting.h"
#include "ChessBoard.h"

namespace fcnpr {

    bool PandR::init_first_level() {
        auto initial_nodes = network().nodes_at_level(0);
        auto middle = chessboard().size() / 2;

        uint64_t initial_distance = 1;
        std::vector<Position> initial_positions;
        while(true) {
            initial_positions = chessboard().neighbours({middle, middle}, initial_distance);
            if(initial_nodes.size() <= initial_positions.size()) break;
            else ++initial_distance;
        }

        std::unordered_map<Node, std::vector<Position>> candidates;
        for(auto i=0; i<initial_nodes.size(); ++i) {
            candidates[initial_nodes[i]] = initial_positions;
        }

        LevelPlacement level_placement(solution, candidates);
        while(!level_placement.find_next_group_of_positions()) {
            if(level_placement.exhausted())
                return false;
        }
        solution->push_placement(std::move(level_placement));
        solution->push_routing(LevelRouting(solution, 0));
        return true;
    }

    bool PandR::place_nth_level(std::size_t n) {
        LevelPlacement level_placement(solution, n);

        if(level_placement.empty()) return false;

        while (!level_placement.exhausted()) {
            if(level_placement.find_next_group_of_positions()) {
                std::cout << "aa" << std::endl;
                solution->push_placement(std::move(level_placement));
                LevelRouting level_routing(solution, n);
                if(level_routing.wire_current_level_of_routes()) {
                    std::cout << "bb" << std::endl;
                    solution->push_routing(std::move(level_routing));
                    return true;
                }else{
                    solution->pop_placement();
                }
            }
        }
        return false;
    }

    bool PandR::backtrack_previous_levels(std::size_t &n) {
        assert(solution->size()-1 == n);

        bool placed = false;
        bool routed = false;
        while(n>=0) {
            solution->current_routing().unwire_current_level_of_routes();
            solution->current_placement().unplace_current_level_of_nodes();

            placed = false;
            routed = false;
            if(solution->current_placement().find_next_group_of_positions()) {
                std::cout << "qq"<<std::endl;
                placed = true;
                if(solution->current_routing().wire_current_level_of_routes()) {
                    std::cout << "ll"<<std::endl;
                    routed = true;
                }
            }

            if(placed && routed) {
                return true;
            } else {
                if(routed)
                    solution->current_routing().unwire_current_level_of_routes();
                if(placed)
                    solution->current_placement().unplace_current_level_of_nodes();
                solution->pop_routing();
                solution->pop_placement();
                n--;
            }
        }
        return false;
    }


    bool PandR::run() {
        auto start = std::chrono::steady_clock::now();

        init_first_level();

        auto depth = network().depth();
        std::size_t n = 0;

        while(solution->size() != depth) {
            n++;
            if(!place_nth_level(n)) {
                n--;
                if(backtrack_previous_levels(n)) {
                    return false;
                }
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        pandr_duration = std::chrono::duration<double, std::milli>(diff).count();
        return true;
    }

    double PandR::duration() const noexcept {
        return pandr_duration;
    }



}