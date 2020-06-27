//
// Created by fpeng on 2020/6/15.
//

#ifndef ROPPER_LEVELPLACEMENT_H
#define ROPPER_LEVELPLACEMENT_H

#include <set>
#include <unordered_map>
#include <map>
#include <optional>
#include <memory>
#include "Types.h"

namespace fcnpr {

    class Solution;
    constexpr uint32_t LEVEL_MULTIPLIER = 4;

    class LevelPlacement {
    public:
        LevelPlacement(std::shared_ptr<Solution> sln, std::unordered_map<Node, std::vector<Position>> cdt);
        LevelPlacement(std::shared_ptr<Solution> sln, Level);

        std::optional<Position> position(Node node) const noexcept;
        std::map<Node, Position> current_positions() const noexcept;

        bool find_next_group_of_positions() noexcept;

        bool empty() const noexcept;
        bool exhausted() const noexcept;

        bool place_current_level_of_nodes() noexcept;
        void unplace_current_level_of_nodes() noexcept;
    private:
        std::shared_ptr<Solution> solution;

        std::unordered_map<Node, std::vector<Position>> candidates;
        std::map<Node, std::vector<Position>::size_type> current_indices;

    private:
        std::vector<Position> candidate_position_for(Node node) const;
        void advance_current_positions() noexcept;
    };

}



#endif //ROPPER_LEVELPLACEMENT_H
