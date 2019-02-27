#pragma once
#include <utility>
#include <vector>

namespace pandr {
	using Region = std::pair<uint64_t,uint64_t>;
	using Regions = std::vector<std::pair<uint64_t,uint64_t>>;
	using Route = Regions;
	using Routes = std::vector<Regions>;
}
