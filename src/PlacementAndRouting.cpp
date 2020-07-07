//
// Created by fpeng on 2020/7/7.
//

#include "PlacementAndRouting.h"
#include "ChessBoard.h"
#include "Network.h"

namespace fcnpr {
    PlacementAndRouting::PlacementAndRouting() {
        num_level = network().depth()+1;
        level_has_initialized.resize(num_level, false);
        level_has_placed.resize(num_level, false);
        level_has_routed.resize(num_level, false);

        candidate_positions.reserve(num_level);
        level_placement_indices.reserve(num_level);
        level_routings.reserve(num_level);
    }

    bool PlacementAndRouting::run() {
        std::size_t current_level = 0;
        while(current_level < num_level) {
            if(!level_has_initialized[current_level]) {
                initialize_level(current_level);
                level_has_initialized[current_level] = true;

                if(level_is_empty(current_level)) {
                    candidate_positions.pop_back();
                    level_placement_indices.pop_back();
                    level_routings.pop_back();
                    level_has_initialized[current_level] = false;
                    level_has_placed[current_level] = false;
                    level_has_routed[current_level] = false;
                    current_level--;
                    continue;
                }
            }

            if(place_nodes_of_level(current_level)) {
                if(route_nodes_of_level(current_level)) {
                    current_level++;
                }
            } else {
                if(current_level == 0){
                    return false;
                } else {
                    candidate_positions.pop_back();
                    level_placement_indices.pop_back();
                    level_routings.pop_back();
                    level_has_initialized[current_level] = false;
                    level_has_placed[current_level] = false;
                    level_has_routed[current_level] = false;
                    current_level--;
                }
            }
        }

        return true;
    }

    void PlacementAndRouting::pr_result() noexcept {
        std::cout << "Area: " << chessboard().compute_layout_area() << std::endl;
        std::cout << "Primary Inputs: " << std::endl;

        for(auto &[node, index] : level_placement_indices[0]) {
            std::cout << node << " : " << candidate_positions[0][node][index] << std::endl;
        }

        for(std::size_t i=1; i<num_level; ++i) {
            std::cout << "Level " << i << std::endl;
            std::cout << "Node Positions: " << std::endl;
            for(auto &[node, index] : level_placement_indices[i]) {
                std::cout << node << " : " << candidate_positions[i][node][index] << std::endl;
            }
            std::cout << "Routings: " << std::endl;
            for(auto &[src_tgt, route] : level_routings[i]) {
                std::cout << src_tgt.first << " => " << src_tgt.second << " : ";
                for(auto &pos : route) {
                    std::cout << pos << " ";
                }
                std::cout << std::endl;
            }
        }
    }

    void PlacementAndRouting::initialize_level(std::size_t level) {
        auto current_level_nodes = network().nodes_at_level(level);

        if(level == 0) {
            auto middle = chessboard().size() / 2;

            uint64_t initial_distance = 1;
            std::vector<Position> initial_positions;
            while(true) {
                initial_positions = std::move( chessboard().neighbours({middle, middle}, initial_distance) );
                if(current_level_nodes.size() <= initial_positions.size()) break;
                else ++initial_distance;
            }

            std::unordered_map<Node, std::vector<Position>> candidates;
            for(auto i=0; i<current_level_nodes.size(); ++i) {
                candidates[current_level_nodes[i]] = initial_positions;
            }
            candidate_positions.push_back(std::move(candidates));
        } else {
            candidate_positions.emplace_back();

            auto size_xy = chessboard().size();
            std::map<Position,std::size_t> position_occurrence;

            for (auto &node :current_level_nodes) {
                position_occurrence.clear();

                auto fanins = network().fan_ins_of(node);
                for( const auto&fanin : fanins ) {
                    Position fanin_pos = find_fanin_position_for(fanin);
                    auto &x = fanin_pos.first;
                    auto &y = fanin_pos.second;
                    auto distance = network().level_distance(node, fanin) * LEVEL_MULTIPLIER;

                    uint64_t floor_i = (x-distance >= 0)? x-distance : 0;
                    uint64_t floor_j = (y-distance >= 0)? y-distance : 0;
                    uint64_t top_i = (x+distance < size_xy-1)? x+distance : size_xy-1;
                    uint64_t top_j = (y+distance < size_xy-1)? y+distance : size_xy-1;

                    for(auto i=floor_i; i<=top_i; i++){
                        for(auto j=floor_j; j<=top_j; j++){
                            Position cur_pos = {i,j};
                            auto route = chessboard().find_route_between(fanin_pos, cur_pos);

                            if(route.has_value() && route.value().size() == (distance+1)) {
                                position_occurrence[cur_pos]++;
                            }
                        }
                    }
                }

                std::vector<Position> positions;
                for(auto &[pos, occurrences] : position_occurrence) {
                    if(occurrences == fanins.size()) {
                        positions.push_back(pos);
                    }
                }

                candidate_positions[level].insert({node, positions});
            }
        }

        level_placement_indices.emplace_back();
        for(auto &[id, cdts] : candidate_positions.back()) {
            level_placement_indices.back().insert({id, -1});
        }

        level_routings.emplace_back();
    }

    bool PlacementAndRouting::place_nodes_of_level(std::size_t level) noexcept {
        assert(level == (level_placement_indices.size()-1));
        assert(!level_is_empty(level));

        if(level_has_routed[level]) {
            for(auto &[src_tgt, route]: level_routings[level]) {
                chessboard().unwire_route(route);
            }
            level_routings[level].clear();
            level_has_routed[level] = false;
        }

        if(level_has_placed[level]) {
            for(auto &[node, index] : level_placement_indices[level]) {
                chessboard().unplace_node(candidate_positions[level][node][index]);
            }
            level_has_placed[level] = false;
        }


        std::vector<Position> placed_positions;
        while(!level_is_exhausted(level)) {
            advance_level_indices(level);

            level_has_placed[level] = true;
            for(auto&[node, index] : level_placement_indices[level]) {
                auto pos = candidate_positions[level][node][index];
                if( chessboard().place_node(pos, node) ) {
                    placed_positions.push_back(pos);
                } else {
                    for(auto &pos : placed_positions) {
                        chessboard().unplace_node(pos);
                    }
                    placed_positions.clear();
                    level_has_placed[level] = false;
                    break;
                }
            }

            if(level_has_placed[level]) {
                break;
            }
        }

        return level_has_placed[level];
    }

    bool PlacementAndRouting::route_nodes_of_level(std::size_t level) noexcept {
        if(level == 0) {
            return true;
        }

        assert(level_routings.back().empty());
        for(auto &[node, index] : level_placement_indices.back()) {
            auto cur_pos = candidate_positions.back()[node][index];
            for(auto &fanin : network().fan_ins_of(node)) {
                auto fanin_pos = find_fanin_position_for(fanin);
                auto route = chessboard().find_route_between(fanin_pos, cur_pos).value();
                if( chessboard().wire_route(route) ) {
                    level_routings.back()[{fanin, node}] = route;
                    continue;
                } else {
                    for(auto &[src_tgt, route] : level_routings.back())  {
                        chessboard().unwire_route(route);
                    }
                    level_routings[level].clear();
                    level_has_routed[level] = false;
                    break;
                }
            }
        }

        level_has_routed[level] = true;
        return level_has_routed[level];
    }

    Position PlacementAndRouting::find_fanin_position_for(const Node &fanin) const noexcept {
        std::size_t level = level_placement_indices.size() - 1;
        for (auto it = std::crbegin(level_placement_indices); it != std::crend(level_placement_indices); ++it, --level) {
            auto idx_it = it->find(fanin);
            if (idx_it != it->end()) {
                return candidate_positions[level].at(fanin)[idx_it->second];
            }
        }
        assert(false);
        return {-1,-1};
    }

    bool PlacementAndRouting::level_is_empty(std::size_t level) const noexcept {
        bool ret = false;
        for(auto &[id, pos] : candidate_positions.at(level)) {
            if(pos.empty()) {
                ret = true;
                break;
            }
        }
        return ret;
    }

    bool PlacementAndRouting::level_is_exhausted(std::size_t level) const noexcept {
        bool exhausted = true;
        for(auto &[node, pos] : level_placement_indices.at(level)) {
            exhausted = exhausted && (candidate_positions.at(level).at(node).size() == (pos+1));
        }
        return exhausted;
    }

    void PlacementAndRouting::advance_level_indices(std::size_t level) noexcept {
        assert(!level_is_exhausted(level));

        //if(visited[level]) {
        for(auto &[node, pos] : level_placement_indices[level]) {
            pos = (pos+1) % candidate_positions[level].at(node).size();
            if(pos==0) continue;
            else break;
        }
//        } else {
//            visited.back() = true;
//        }
    }


}
