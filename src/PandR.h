//
// Created by fpeng on 2020/6/17.
//

#ifndef ROPPER_PANDR_H
#define ROPPER_PANDR_H

#include <chrono>

#include "ChessBoard.h"
#include "Network.h"
#include "Solution.h"

namespace fcnpr {

    class PandR {
    public:
        bool init_first_level();
        bool place_nth_level(std::size_t n);
        bool backtrack_previous_levels(std::size_t &n);
        bool run();

        double duration() const noexcept;
        Json PandR::json() const noexcept;

        inline solution &get_solution() noexcept {
            return solution;
        }
        inline const solution &get_solution() const noexcept {
            return solution;
        }

    private:
        Solution solution;
        double pandr_duration{0};
    };

}


#endif //ROPPER_PANDR_H
