#pragma once
#include <algorithm>
#include <vector>
#include <algorithms/Placements.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class Blacklist {
		private:
			std::vector<Placements<Matrix>> blacklist;
		public:
			Blacklist() = default;
			void add(Placements<Matrix> const& src) noexcept;
			bool contains(Placements<Matrix> const& src) const noexcept;
	};

	template<typename Matrix>
	void Blacklist<Matrix>::add(Placements<Matrix> const& src) noexcept {
		this->blacklist.push_back(src);
	}

	template<typename Matrix>
	bool Blacklist<Matrix>::contains(Placements<Matrix> const& src) const noexcept {
		auto search = std::find(std::begin(this->blacklist), std::end(this->blacklist), src);
		return (search != std::end(this->blacklist));
	}
}
