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

    static const uint64_t DEFAULT_SIZE = 100;

    class ChessBoard {
    public:
        ChessBoard(uint64_t sz);

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

        bool place_node(const Position &pos, Node node) noexcept;
        void unplace_node(const Position &pos) noexcept;
        bool wire_route(const Route &wire) noexcept;
        void unwire_route(const Route &wire) noexcept;

        inline uint8_t time_zone_of(const Position &pos) const noexcept {
            return (y % 2 != 0)?
                   ( ((y+1)%4 != 0)? (1+(x+1)%4) : (1+(x+3)%4) ) :
                   ( (y%4 == 0)? (4-(x+3)%4) : (4-((x+1)%4)) );
        }
        bool exists_datapath_between(const Position &pos1, const Position &pos2) const noexcept;
        uint64_t compute_layout_area() const noexcept;
        std::optional<const Route&> find_route_between(const Position &pos1, const Position &pos2) const noexcept;
        Region neighbours(const Position &center, uint64_t radius) const noexcept;


    private:
        uint64_t    grid_size;
        std::vector<std::vector<Cell>> cell_grid;

        std::map<Coordinate, std::size_t> x_positions;
        std::map<Coordinate, std::size_t> y_positions;

        std::unordered_map<std::pair<Position, Position>, Route> paths_in_grid;

        void establish_paths_cache();
        Route compute_path_between(const Position &pos1, const Position &pos2);

        void place_callback(const Position &pos) noexcept;
        void unplace_callback(const Position &pos) noexcept;

        friend ChessBoard& get_chessboard();
    };

    ChessBoard &chessboard() {
        static ChessBoard instance{DEFAULT_SIZE};
        return instance;
    }

}


#endif //ROPPER_CHESSBOARD_H
