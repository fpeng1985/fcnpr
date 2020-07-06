//
// Created by fpeng on 2020/6/14.
//

#include "ChessBoard.h"
#include <queue>
#include <cassert>


namespace fcnpr {

    ChessBoard::ChessBoard(uint64_t sz)  : grid_size(sz), cell_grid(sz, std::vector<Cell>(sz))
    {establish_paths_cache();}

    uint64_t ChessBoard::size() const noexcept {
        return grid_size;
    }

    void ChessBoard::clear() noexcept {
        x_positions.clear();
        y_positions.clear();
        cell_grid.clear();
        grid_size = 0;
    }

    void ChessBoard::resize(uint64_t sz) {
        grid_size = sz;
        cell_grid.resize(sz);
        for(auto &column : cell_grid) {
            column.resize(sz);
        }
    }

    bool ChessBoard::place_node(const Position &pos, Node node) noexcept {
        if( cell_at(pos).put_node(node) ){
            place_callback(pos);
            return true;
        }
        return false;
    }

    void ChessBoard::unplace_node(const Position &pos) noexcept {
        if(has_node_in(pos)) {
            cell_at(pos).unput_node();
            unplace_callback(pos);
        }
    }

    bool ChessBoard::wire_route(const Route &route) noexcept {
        Route wired_route;
        for(auto it{std::begin(route)}; it!=std::end(route); ++it){
            if(it == std::begin(route)) continue;
            if((it+1) == std::end(route)) break;

            auto &pos = *it;
            //std::cout << "aa" << std::endl;
            if(cell_at(pos).put_wire()) {
                //std::cout << "xxx" <<std::endl;
                wired_route.push_back(pos);
                place_callback(pos);
            } else {
                //std::cout << unsigned (chessboard().cell_at(pos).current_weight) <<std::endl;
                unwire_route(wired_route);
                //std::cout <<"yy" <<std::endl;
                return false;
            }
        }
        return true;
    }

    void ChessBoard::unwire_route(const Route &route) noexcept {
        for(auto it{std::begin(route)}; it!=std::end(route); ++it) {
            if(it == std::begin(route)) continue;
            if((it+1) == std::end(route)) break;

            auto &pos = *it;

            cell_at(pos).unput_wire();
            unplace_callback(pos);
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

    std::optional<Route> ChessBoard::find_route_between(const Position &pos1, const Position &pos2) const noexcept {
        assert(pos1.first  < grid_size);
        assert(pos1.second < grid_size);
        assert(pos2.first  < grid_size);
        assert(pos2.second < grid_size);

        if(pos1 == pos2) {
            return std::nullopt;
        } else {
            return paths_in_grid.at({pos1,pos2});
        }
    }

    std::vector<Position> ChessBoard::neighbours(const Position &center, uint64_t radius) const noexcept {
        std::vector<Position> ret;

        auto &x = center.first;
        auto &y = center.second;

        uint64_t floor_i = (x-radius >= 0)? x-radius : 0;
        uint64_t floor_j = (y-radius >= 0)? y-radius : 0;
        uint64_t top_i = (x+radius < grid_size-1)? x+radius : grid_size-1;
        uint64_t top_j = (y+radius < grid_size-1)? y+radius : grid_size-1;

        for(auto i=floor_i; i<=top_i; i++){
            for(auto j=floor_j; j<=top_j; j++){
                Position cur_pos = {i,j};
                auto route{chessboard().find_route_between(center, cur_pos)};
                if(route.has_value() && route.value().size() == (radius+1)) {
                    ret.push_back(cur_pos);
                }
//                if(chessboard().find_route_between(center, cur_pos).value().size() == (radius + 1)){
//                    ret.push_back(cur_pos);
//                }
            }
        }

        return ret;
    }

    void ChessBoard::establish_paths_cache() {
        paths_in_grid.clear();

        int64_t gsz = static_cast<int64_t>(grid_size);

        for(int64_t x1=0; x1<gsz; ++x1) {
            for(int64_t y1=0; y1<gsz; ++y1) {
                for(int64_t x2=gsz-1; x2>=0; --x2) {
                    for(int64_t y2=gsz-1; y2>=0; --y2) {
                        if(x1==x2 && y1==y2) continue;
                        Position pos1 = {x1, y1};
                        Position pos2 = {x2, y2};
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
        std::queue<Position> position_queue;
        std::queue<Route>    route_queue;
        std::vector<std::vector<bool>> visited(grid_size, std::vector<bool>(grid_size, false));


        position_queue.push(pos1);
        route_queue.push({pos1});
        visited[pos1.first][pos1.second] = true;

        while(!position_queue.empty()) {

            auto current_position = position_queue.front(); position_queue.pop();
            auto current_route    = route_queue.front(); route_queue.pop();

            if(current_position == pos2) {
                ret = current_route;
                break;
            }

            auto x = std::get<0>(current_position);
            auto y = std::get<1>(current_position);

            auto move = [&](const Position &pos){
                current_route.push_back(pos);
                position_queue.push(pos);
                route_queue.push(current_route);
                visited[pos.first][pos.second] = true;
                paths_in_grid[{pos1, pos}] = current_route;
                current_route.pop_back();
            };


            if(x>0) {
                Position leftward{x-1, y};
                if(visited[x-1][y]==false && exists_datapath_between(current_position, leftward)) {
                    move(leftward);
                }
            }

            if(x<grid_size-1) {
                Position rightward = {x+1, y};
                if(visited[x+1][y] == false && exists_datapath_between(current_position, rightward)) {
                    move(rightward);
                }
            }

            if(y<grid_size-1) {
                Position upward = {x, y+1};
                if(visited[x][y+1] == false && exists_datapath_between(current_position, upward)) {
                    move(upward);
                }
            }

            if(y>0) {
                Position downward = {x, y - 1};
                if (visited[x][y - 1] == false && exists_datapath_between(current_position, downward)) {
                    move(downward);
                }
            }
        }
        return ret;
    }

    void ChessBoard::place_callback(const Position &pos) noexcept {
        auto search_x {x_positions.find(pos.first)};
        auto search_y {y_positions.find(pos.second)};

        if(search_x == std::end(x_positions)) x_positions.insert({pos.first,1});
        else search_x->second++;

        if(search_y == std::end(y_positions)) y_positions.insert({pos.second,1});
        else search_y->second++;
    }

    void ChessBoard::unplace_callback(const Position &pos) noexcept {
        auto search_x {x_positions.find(pos.first)};
        auto search_y {y_positions.find(pos.second)};

        if(search_x == std::end(x_positions)) return;
        if(search_y == std::end(y_positions)) return;

        if(search_x->second<= 1) x_positions.erase(search_x);
        else search_x->second--;

        if(search_y->second<= 1) y_positions.erase(search_y);
        else search_y->second--;
    }

    ChessBoard &chessboard() {
        static ChessBoard instance{DEFAULT_SIZE};
        return instance;
    }

}

