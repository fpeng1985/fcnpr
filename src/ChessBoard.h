//
// Created by fpeng on 2020/6/14.
//

#ifndef ROPPER_CHESSBOARD_H
#define ROPPER_CHESSBOARD_H

#include <functional>
#include <map>
#include <unordered_map>
#include <cassert>

#include "Types.h"

namespace fcnpr {

using ClockScheme = std::function<TimeZone(Position)>;
using PathFinder  = std::function<Route(Position, Position)>;
using CallBacks   = std::vector<std::function<void(const Position&)>>;

class ChessBoard {
public:
    ChessBoard(uint64_t sz, PathFinder pf);

    uint64_t size() const noexcept;
    void clear() noexcept;
    void resize();

    inline Cell&       cell_at(const Position &pos) noexcept {
        assert(pos.first < grid_size && pos.second < grid_size);
        return cell_grid[pos.first][pos.second];
    }
    inline Cell const& cell_at(const Position &pos) const noexcept {
        assert(pos.first < grid_size && pos.second < grid_size);
        return cell_grid[pos.first][pos.second];
    }
    inline bool is_empty_in(const Position &pos) const noexcept {
        assert(pos.first < grid_size && pos.second < grid_size);
        return cell_grid[pos.first][pos.second].empty();
    }
    inline bool has_node_in(const Position &pos) const noexcept {
        assert(pos.first < grid_size && pos.second < grid_size);
        return cell_grid[pos.first][pos.second].has_node();
    }

    bool place_node(const Position &pos, Id node);
    bool unplace_node(const Position &pos);
    bool place_wire(const Route &route);
    void unplace_wire(const Route &route);

    inline uint8_t time_zone_of(const Position &pos) const noexcept {
        return (y % 2 != 0)?
               ( ((y+1)%4 != 0)? (1+(x+1)%4) : (1+(x+3)%4) ) :
               ( (y%4 == 0)? (4-(x+3)%4) : (4-((x+1)%4)) );
    }
    bool exists_datapath_between(const Position &pos1, const Position &pos2) const noexcept;
    uint64_t compute_layout_area() const noexcept;
    std::optional<const Route&> find_path_between(const Position &pos1, const Position &pos2) const noexcept;


private:
    uint64_t    grid_size;
    PathFinder  path_finder;
    std::vector<std::vector<Cell>> cell_grid;

    std::map<Coordinate, std::size_t> x_positions;
    std::map<Coordinate, std::size_t> y_positions;

    CallBacks place_callbacks;
    CallBacks unplace_callbacks;

    std::unordered_map<std::pair<Position, Position>, Route> paths_in_grid;

    inline void execute_callbacks(CallBacks &callbacks, const Position &pos) {
        for(auto &cb : callbacks) {
            cb(pos);
        }
    }

    void establish_paths_cache();
    Route compute_path_between(const Position &pos1, const Position &pos2);

    friend ChessBoard& get_chessboard();
};

ChessBoard &get_chessboard() {
    static ChessBoard instance;
    return instance;
}

}


#endif //ROPPER_CHESSBOARD_H
