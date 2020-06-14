#pragma once
#include <utility>
#include <vector>
#include <set>

namespace pandr {
	using Region = std::pair<uint64_t,uint64_t>;
	using Regions = std::vector<std::pair<uint64_t,uint64_t>>;
	using Route = Regions;
	using Routes = std::vector<Regions>;
}

namespace fcnpr {
    using Id         = uint64_t;
    using Coordinate = uint64_t;
    using Position   = std::pair<Coordinate, Coordinate>;
    using Region     = std::set<Position>;
    using Placement  = std::pair<Id, Position>;
    using Route      = std::vector<Position>;
    using TimeZone   = uint8_t;
}
