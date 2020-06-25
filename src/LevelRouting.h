//
// Created by fpeng on 2020/6/23.
//

#ifndef ROPPER_LEVELROUTING_H
#define ROPPER_LEVELROUTING_H

#include <memory>
#include <map>
#include "Types.h"

namespace fcnpr {

    class Solution;

    class LevelRouting {
    public:
        LevelRouting(std::shared_ptr<Solution> sln, Level l);

        bool wire_current_level_of_routes() noexcept;
        void unwire_current_level_of_routes() noexcept;

        inline std::map<std::pair<Node, Node>, Route>::iterator begin() noexcept {
            return routings.begin();
        }
        inline std::map<std::pair<Node, Node>, Route>::const_iterator begin() const noexcept {
            return routings.begin();
        }
        inline std::map<std::pair<Node, Node>, Route>::iterator end() noexcept {
            return routings.end();
        }
        inline std::map<std::pair<Node, Node>, Route>::const_iterator end() const noexcept {
            return routings.end();
        }

    private:
        std::shared_ptr<Solution> solution;

        Level level;
        std::map<std::pair<Node, Node>, Route> routings;
    };

}

#endif //ROPPER_LEVELROUTING_H
