#pragma once
#include <map>
#include <Types.hpp>

namespace pandr::area {
	class Area {
		using X = std::map<uint64_t, uint64_t>;
		using Y = std::map<uint64_t, uint64_t>;

		private:
			X x_map;
			Y y_map;
		public:
			Area() = default;
			void set(uint64_t x, uint64_t y) noexcept;
			void unset(uint64_t x, uint64_t y) noexcept;
			uint64_t get() const noexcept;
	};

	void Area::set(uint64_t x, uint64_t y) noexcept {
		auto search_x {x_map.find(x)};
		auto search_y {y_map.find(y)};
		
		if(search_x == std::end(x_map)) x_map.insert({x,1});
		else search_x->second++;

		if(search_y == std::end(y_map)) y_map.insert({y,1});
		else search_y->second++;
	}

	void Area::unset(uint64_t x, uint64_t y) noexcept {
		auto search_x {x_map.find(x)};
		auto search_y {y_map.find(y)};
		
		if(search_x == std::end(x_map)) return;
		if(search_y == std::end(y_map)) return;

		if(search_x->second<= 1) x_map.erase(search_x);
		else search_x->second--;

		if(search_y->second<= 1) y_map.erase(search_y);
		else search_y->second--;
	}

	uint64_t Area::get() const noexcept {
		if(x_map.empty()) return 0;

		auto x_max {std::rbegin(x_map)->first};
		auto x_min {std::begin(x_map)->first};
		auto x_delta{(x_max+1)-x_min};

		auto y_max {std::rbegin(y_map)->first};
		auto y_min {std::begin(y_map)->first};
		auto y_delta{(y_max+1)-y_min};

		return (x_delta*y_delta);
	}
} /* pandr::area namespace */
