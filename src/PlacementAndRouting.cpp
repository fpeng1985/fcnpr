//
// Created by Administrator on 2020/7/7.
//

#include "PlacementAndRouting.h"

namespace fcnpr{

    PlacementAndRouting::PlacementAndRouting() {}

    std::optional<Position> PlacementAndRouting::position(Node node,levelplacement lp) const noexcept {
        auto it = lp.current_indices.find(node);
        if (it == lp.current_indices.end())
            return std::nullopt;
        else {
            auto idx = it->second;
            return lp.candidates.at(node).at(idx);
        }
    }

    std::map<Node, Position> PlacementAndRouting::current_positions(levelplacement lp) const noexcept {

        std::map<Node, Position> positions;
        for(auto &[node, idx] : lp.current_indices) {
            positions[node] = lp.candidates.at(node).at(idx);
        }
        return positions;
    }

    std::map<Node, Position> PlacementAndRouting::find_level_positions(Level level) const noexcept {
        assert(level < placements.size());
        return current_positions(placements[level]);
    }

    std::optional<Position> PlacementAndRouting::find_position(Node node) const noexcept {
        std::optional<Position> ret = std::nullopt;

        for(auto it=std::crbegin(placements); it!=std::crend(placements); ++it) {
            if (position(node,*it).has_value()) {
                auto pos = position(node,*it).value();
                ret = pos;
                break;
            }
        }
        return ret;
    }

    bool PlacementAndRouting::empty(levelplacement lp) const noexcept {
        bool ret = false;
        for(auto &[id, cdt] : lp.candidates) {
            if(cdt.empty()) ret = true;
        }
        return ret;
    }

    bool PlacementAndRouting::exhausted(levelplacement lp) const noexcept {
        bool exhausted = true;
        for(auto &[node, pos] : lp.current_indices) {
            exhausted = exhausted && (lp.candidates.at(node).size() == (pos+1));
        }
        return exhausted;
    }

    bool PlacementAndRouting::place_current_level_of_nodes(levelplacement lp) noexcept {
        std::vector<Position> placed_positions;
        bool roll_back = false;

        for(auto&[node, index] : lp.current_indices) {
            if( chessboard().place_node(lp.candidates[node][index], node) ) {
                placed_positions.push_back(lp.candidates[node][index]);
            } else {
                roll_back = true;
                break;
            }
        }

        if(roll_back) {
            for(auto &pos : placed_positions) {
                chessboard().unplace_node(pos);
            }
            return false;
        }
        return true;
    }

    void PlacementAndRouting::unplace_current_level_of_nodes(levelplacement lp) noexcept {
        for(auto &[node,index] : lp.current_indices) {
            auto pos = lp.candidates[node][index];
            chessboard().unplace_node(pos);
        }
    }

    std::vector<Position> PlacementAndRouting::candidate_position_for(Node node) const {
        auto size_xy = chessboard().size();
        std::map<Position,std::size_t> position_occurrence;

        auto fanins = network().fan_ins_of(node);
        Position fanin_pos;

        for( const auto&fanin : fanins ) {
            assert(find_position(fanin).has_value());
            if(find_position(fanin).has_value()){
                fanin_pos = find_position(fanin).value();
            }
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

        return positions;
    }

    void PlacementAndRouting::advance_current_positions(levelplacement lp) noexcept {
        assert(!exhausted(lp));

        std::vector<Position>::size_type sum = 0;
        for(auto &[id, pos] : lp.current_indices) {
            sum += pos;
        }

        //if(sum!=0) {
        for(auto &[id, pos] : lp.current_indices) {
            pos = (pos+1) % lp.candidates[id].size();
            if(pos==0) continue;
            else break;
        }
        //}
    }

    bool PlacementAndRouting::find_next_group_of_positions(levelplacement lp) noexcept {
        assert(!empty(lp));

        while(!exhausted(lp)) {
            advance_current_positions(lp);
            if(place_current_level_of_nodes(lp))
                return true;
        }

        return false;
    }

    //routing
    bool PlacementAndRouting::wire_current_level_of_routes(levelrouting lr) noexcept {
        if(lr.level == 0){
            return true;
        }

        auto nodes = network().nodes_at_level(lr.level);
        auto current_level_positions  = find_level_positions(lr.level);
        for(auto &node : nodes) {
            auto fanins = network().fan_ins_of(node);

            for(auto &fanin : fanins) {
                Position cur_pos = current_level_positions[node];
                Position fanin_pos = find_position(fanin).value();
                auto route = chessboard().find_route_between(cur_pos, fanin_pos).value();
                if( chessboard().wire_route(route) ) {
                    lr.routes[{node, fanin}] = route;
                    continue;
                }
                else {
                    for(auto &[src_tgt, route]: lr.routes) {
                        chessboard().unwire_route(route);
                    }
                    lr.routes.clear();
                    return false;
                }
            }
        }

        return true;
    }

    void PlacementAndRouting::unwire_current_level_of_routes(levelrouting lr) noexcept {
        if(lr.level == 0){
            return;
        }
        for(auto &[src_tgt, route] : lr.routes) {
            chessboard().unwire_route(route);
        }
        lr.routes.clear();
    }

    //pandr
    bool PlacementAndRouting::init_first_level() {
        auto initial_nodes = network().nodes_at_level(0);
        auto middle = chessboard().size() / 2;

        uint64_t initial_distance = 1;
        std::vector<Position> initial_positions;
        while(true) {
            initial_positions = chessboard().neighbours({middle, middle}, initial_distance);
            if(initial_nodes.size() <= initial_positions.size()) break;
            else ++initial_distance;
        }
        levelplacement lp0;
        for(auto i=0; i<initial_nodes.size(); ++i) {
            lp0.candidates[initial_nodes[i]] = initial_positions;
        }
        for(auto &[id,positions] : lp0.candidates) {
            lp0.current_indices.insert({id,0});
        }
        lp0.level = 0;

        while(find_next_group_of_positions(lp0)) {
            if(exhausted(lp0))
                return false;
        }

        levelrouting lr0;
        lr0.level = 0;
        placements.push_back(lp0);
        routings.push_back(lr0);
        return true;
    }

    bool PlacementAndRouting::place_nth_level(std::size_t n) {
        assert(n>0);

        levelplacement lp;
        auto nodes = network().nodes_at_level(n);
        for(auto node:nodes) {
            lp.current_indices.insert({node, 0});
            lp.candidates.insert({node, candidate_position_for(node)});
        }
        lp.level = 0;

        while(find_next_group_of_positions(lp)) {
            if(exhausted(lp))
                return false;
        }

        if(empty(lp)) return false;

        while (exhausted(lp)) {
            if(find_next_group_of_positions(lp)) {
                placements.push_back(lp);
                levelrouting lr;
                lr.level = n;
                if(wire_current_level_of_routes(lr)) {
                    routings.push_back(lr);
                    return true;
                }else{
                    placements.pop_back();
                }
            }
        }
        return false;
    }

    bool PlacementAndRouting::backtrack_previous_levels(std::size_t &n) {
        assert(placements.size()-1 == n);

        bool placed = false;
        bool routed = false;
        while(n>=0) {
            unwire_current_level_of_routes(current_routing());
            unplace_current_level_of_nodes(current_placement());

            placed = false;
            routed = false;
            if(find_next_group_of_positions(current_placement())) {
                placed = true;
                if(wire_current_level_of_routes(current_routing())) {
                    routed = true;
                }
            }

            if(placed && routed) {
                return true;
            } else {
                if(routed)
                    unwire_current_level_of_routes(current_routing());
                if(placed)
                    unplace_current_level_of_nodes(current_placement());
                routings.pop_back();
                placements.pop_back();
                n--;
            }
        }
        return false;
    }

    bool PlacementAndRouting::run() {
        auto start = std::chrono::steady_clock::now();

        init_first_level();

        auto depth = network().depth();
        std::size_t n = 0;

        while(placements.size() != depth) {
            std::cout << n << std::endl;
            n++;
            if(!place_nth_level(n)) {
                n--;
                if(!backtrack_previous_levels(n)) {
                    return false;
                }
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        pandr_duration = std::chrono::duration<double, std::milli>(diff).count();
        return true;
    }

    double PlacementAndRouting::duration() const noexcept {
        return pandr_duration;
    }

    Json PlacementAndRouting::json() const noexcept {
        Json j;

        j["fcnpr"]["area"] = chessboard().compute_layout_area();
        auto node_positions = find_level_positions(0);
        for (auto const&[node, pos] : node_positions) {
            j["fcnpr"]["0"][std::to_string(node).c_str()] = pos;
        }

        for (std::size_t level = 1; level < routings.size(); ++level) {
            std::string level_str{std::to_string(level)};

            for (auto const &[src_tgt, route] : routings.at(level).routes) {
                std::string fo_str{std::to_string(src_tgt.first)};
                std::string fi_str{std::to_string(src_tgt.second)};
                j["fcnpr"][level_str.c_str()][(fi_str + " -> " + fo_str).c_str()] = route;
            }
        }

        return j;
    }
}
