//
// Created by fpeng on 2020/6/17.
//

#ifndef ROPPER_PANDR_H
#define ROPPER_PANDR_H

#include <chrono>
#include <memory>

#include "ChessBoard.h"
#include "Network.h"
#include "Solution.h"

namespace fcnpr {

    class PandR {
    public:
        PandR() {
            solution = std::make_shared<Solution>();
        }
        bool init_first_level();
        bool place_nth_level(std::size_t n);
        bool backtrack_previous_levels(std::size_t &n);
        bool run();

        double duration() const noexcept;
        //Json json() const noexcept;

        inline Solution &get_solution() noexcept {
            return *solution;
        }
        inline const Solution &get_solution() const noexcept {
            return *solution;
        }

    private:
        std::shared_ptr<Solution> solution;
        double pandr_duration{0};
    };

}


#endif //ROPPER_PANDR_H
