#pragma once
#include <Types.hpp>
#include <algorithms/I_Algorithm.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class Routing : public Algorithm<Matrix> {
		public:
			Routing() = delete;
			Routing(Matrix const& matrix);
			virtual Path run(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) const = 0;
			virtual ~Routing() = default;
	};

	template<typename Matrix>
	Routing<Matrix>::Routing(Matrix const& matrix)
		: Algorithm<Matrix>(matrix)
	{
	}

} /* pandr::algorithm namespace */
