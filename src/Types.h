//
// Created by fpeng on 2020/6/22.
//

#ifndef ROPPER_TYPES_H
#define ROPPER_TYPES_H

#include <utility>
#include <vector>
#include <set>
#include <cstdint>

namespace fcnpr {
    using Node       = uint64_t;
    using Coordinate = uint64_t;
    using Position   = std::pair<Coordinate, Coordinate>;
    using Placement  = std::pair<Node, Position>;
    using Route      = std::vector<Position>;
    using Region     = std::set<Position>;
    using TimeZone   = uint8_t;
    using Level      = uint32_t;
}

#endif //ROPPER_TYPES_H
