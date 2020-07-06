//
// Created by fpeng on 2020/6/14.
//

#ifndef ROPPER_CHESSBOARD_H
#define ROPPER_CHESSBOARD_H

#include <functional>
#include <map>
#include <unordered_map>
#include <cassert>
#include <boost/container_hash/hash.hpp>

#include "Types.h"
#include "Cell.h"

namespace std {
    template<>
    struct hash< std::pair<fcnpr::Position, fcnpr::Position> > {
        using argument_type = std::pair<fcnpr::Position, fcnpr::Position>;
        using result_type   = std::size_t;

        result_type operator()(argument_type const &src_tgt) const noexcept {
            std::size_t seed = 0;
            boost::hash_combine(seed, src_tgt.first.first);
            boost::hash_combine(seed, src_tgt.first.second);
            boost::hash_combine(seed, src_tgt.second.first);
            boost::hash_combine(seed, src_tgt.second.second);
            return seed;
        }

    };
}

namespace fcnpr {

    static const uint64_t DEFAULT_SIZE = 50;

    class ChessBoard {
    public:

        uint64_t size() const noexcept;
        void clear() noexcept;
        void resize(uint64_t sz);

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
            auto const &x = pos.first;
            auto const &y = pos.second;
            return (y % 2 != 0)?
                   ( ((y+1)%4 != 0)? (1+(x+1)%4) : (1+(x+3)%4) ) :
                   ( (y%4 == 0)? (4-(x+3)%4) : (4-((x+1)%4)) );
        }
        bool exists_datapath_between(const Position &pos1, const Position &pos2) const noexcept;
        uint64_t compute_layout_area() const noexcept;
        std::optional<Route> find_route_between(const Position &pos1, const Position &pos2) const noexcept;
        std::vector<Position> neighbours(const Position &center, uint64_t radius) const noexcept;


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

        explicit  ChessBoard(uint64_t sz);
        friend ChessBoard& chessboard();
    };

    ChessBoard &chessboard();
}


#endif //ROPPER_CHESSBOARD_H
