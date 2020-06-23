//
// Created by fpeng on 2020/6/14.
//

#include "ChessBoard.h"
#include <queue>

namespace fcnpr {

    ChessBoard::ChessBoard(uint64_t sz, PathFinder pf) :
            gird_size(sz),
            path_finder(pf), cell_grid(size, std::vector<Cell>(size))
    {
        place_callbacks.push_back([&](const Position &pos){
            auto search_x {x_positions.find(pos->first)};
            auto search_y {y_positions.find(pos->second)};

            if(search_x == std::end(x_positions)) x_positions.insert({x,1});
            else search_x->second++;

            if(search_y == std::end(y_positions)) y_positions.insert({y,1});
            else search_y->second++;
        });

        unplace_callbacks.push_back([&](const Position &pos){
            auto search_x {x_positions.find(pos->first)};
            auto search_y {y_positions.find(pos->second)};

            if(search_x == std::end(x_positions)) return;
            if(search_y == std::end(y_positions)) return;

            if(search_x->second<= 1) x_positions.erase(search_x);
            else search_x->second--;

            if(search_y->second<= 1) y_positions.erase(search_y);
            else search_y->second--;
        });
    }

    uint64_t ChessBoard::size() const noexcept {
        return grid_size;
    }

    void ChessBoard::clear() noexcept {
        x_positions.clear();
        y_positions.clear();
        cell_grid.clear();
        grid_size = 0;
    }

    void resize(uint64_t sz) {
        gird_size = sz;
        cell_grid.resize(sz);
        for(auto &column : cell_grid) {
            column.resize(sz);
        }
    }

    bool ChessBoard::place_node(const Position &pos, Id node) {
        if( cell_at(pos).place_node(node) ){
//            execute_callbacks(place_callbacks, pos);
            x_positions[pos->first]++;
            y_positions[pos->second]++;
            return true;
        }
        return false;
    }

    bool ChessBoard::unplace_node(const Position &pos) {
        if(has_node_in(pos)) {
            cell_at(pos).unplace_node();
            execute_callbacks(unplace_callbacks, pos);
            return true;
        }
        return false;
    }

    bool ChessBoard::place_wire(const Route &route) {
        Route wired_route;
        for(auto it{std::begin(route)}; it!=std::end(route); ++it){
            if(it == std::begin(route)) continue;
            if((it+1) == std::end(route)) break;

            auto &pos = *it;

            if(cell_at(pos).place_wire()) {
                wired_route.push_back(pos);
                execute_callbacks(place_callbacks, pos);
                return true;
            } else {
                unplace_wire(wired_route);
                return false;
            }
        }
    }

    void ChessBoard::unplace_wire(const Route &route) {
        for(auto it{std::begin(route)}; it!=std::end(route); ++it){
            if(it == std::begin(route)) continue;
            if((it+1) == std::end(route)) break;

            auto &pos = *it;

            cell_at(pos).unplace_wire();
            execute_callbacks(unplace_callbacks, pos);
        }
    }

    bool ChessBoard::exists_datapath_between(const Position &pos1, const Position &pos2) const noexcept {
        if( time_zone_of(pos1) == 1 && time_zone_of(pos2) == 4 ) return false;
        if( time_zone_of(pos1) == 4 && time_zone_of(pos2) == 1 ) return true;
        return ( time_zone_of(pos1) < time_zone_of(pos2) );
    }

    uint64_t ChessBoard::compute_layout_area() const noexcept {
        if(x_positions.empty()) return 0;

        auto x_max {std::rbegin(x_positions)->first};
        auto x_min {std::begin(x_positions)->first};
        auto x_delta{(x_max+1)-x_min};

        auto y_max {std::rbegin(y_positions)->first};
        auto y_min {std::begin(y_positions)->first};
        auto y_delta{(y_max+1)-y_min};

        return (x_delta*y_delta);
    }

    std::optional<const Route &> ChessBoard::find_path_between(const Position &pos1, const Position &pos2) const noexcept {
        assert(pos1.first  < grid_size);
        assert(pos1.second < grid_size);
        assert(pos2.first  < grid_size);
        assert(pos2.second < grid_size);

        if(src == dest) {
            return std::nullopt;
        } else {
            return paths_in_grid[{pos1,pos2}];
        }
    }

    Region ChessBoard::neighbours(const Position &center, uint64_t radius) const noexcept {
        Region ret;

        auto &x = center->first;
        auto &y = center->second;

        uint64_t floor_i = (x-radius >= 0)? x-radius : 0;
        uint64_t floor_j = (y-radius >= 0)? y-radius : 0;
        uint64_t top_i = (x+radius < grid_size-1)? x+radius : grid_size-1;
        uint64_t top_j = (y+radius < grid_size-1)? y+radius : grid_size-1;

        for(auto i=floor_i; i<=top_i; i++){
            for(auto j=floor_j; j<=top_j; j++){
                auto cur_pos = {i,j};
                if(chess_board.find_path_between(fanins_pos, cur_pos) == (radius + 1)){
                    ret.push_back(cur_pos);
                }
            }
        }

        return ret;
    }

    void ChessBoard::establish_path_cache() {
        paths_in_grid.clear();

        for(auto x1=0; x1<grid_size; ++x1) {
            for(auto y1=0; y1<grid_size; ++y1) {
                for(auto x2=grid_size-1; x2>=0; --x2) {
                    for(auto y2=grid_size-1; y2>=0; --y2) {
                        if(x1==x2 && y1==y2) continue;
                        auto pos1 = {x1, y1};
                        auto pos2 = {x2, y2};
                        paths_in_grid.insert({{pos1,pos2}, compute_path_between(pos1, pos2)});
                    }
                }
            }
        }
    }

    Route ChessBoard::compute_path_between(const Position &pos1, const Position &pos2) {
        auto it = paths_in_grid.find({pos1, pos2});
        if(it!=paths_in_grid.end()) return it->second;

        Route ret;
        queue<Position> position_queue;
        queue<Route>    route_queue;
        vector<vector<bool>> visited(grid_size, vector<bool>(grid_size, false));

        position_queue.push(pos1);
        route_queue.push({pos1});
        visited[pos1->first][pos1->second] = true;

        while(!position_queue.empty()){
            auto current_position = position_queue.front(); position_queue.pop();
            auto current_route    = route_queue.front(); route_queue.pop();

            if(current_position == pos2) {
                ret = current_route;
                break;
            }

            auto update_cache = [&](Region const& dest, Route const& route){
                c.at(src).insert({dest, route});
            };

            auto x = get<0>(current_position);
            auto y = get<1>(current_position);

            auto move = [&](const Position &pos){
                current_route.push_back(pos);
                position_queue.push(pos);
                route_queue.push(current_route);
                visited[x][y] = true;
                paths_in_grid[{pos1, pos2}] = current_route;
                current_route.pop_back();
            };

            auto leftward = {x-1, y};
            if(x-1 >= 0   && visited[x-1][y] == false && exists_datapath_between(current_position, leftward)) {
                move(leftward);
            }

            auto rightward = {x+1, y};
            if(x+1 < size && visited[x+1][y] == false && exists_datapath_between(current_position, rightward)) {
                move(rightward);
            }

            auto upward = {x, y+1};
            if(y+1 < size && visited[x][y+1] == false && exists_datapath_between(current_position, upward)) {
                move(upward);
            }

            auto downward = {x, y-1};
            if(y-1 >= 0   && visited[x][y-1] == false && exists_datapath_between(current_position, downward)) {
                move(downward);
            }
        }
        return ret;
    }

}

