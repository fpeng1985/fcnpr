//
// Created by fpeng on 2020/6/23.
//

#ifndef ROPPER_LEVELROUTING_H
#define ROPPER_LEVELROUTING_H

#include <memory>

namespace fcnpr {

    class Solution;

    class LevelRouting {
        LevelRouting(std::shared_ptr<Solution> sln, Level l);

        bool wire_current_level_of_routes() noexcept;
        void unwire_current_level_of_routes() noexcept;

    private:
        std::shared_ptr<Solution> solution;

        Level level;
        std::map<std::pair<Id, Id>, Route> routings;
    };

}

#endif //ROPPER_LEVELROUTING_H
