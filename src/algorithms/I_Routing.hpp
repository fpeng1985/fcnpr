#pragma once
#include <Types.hpp>
#include <algorithms/I_Algorithm.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class I_Routing : public I_Algorithm<Matrix> {
		public:
			I_Routing() = delete;
			I_Routing(Matrix& matrix);
			virtual Path run(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) const = 0;
			virtual ~I_Routing() = default;
	};

	template<typename Matrix>
	I_Routing<Matrix>::I_Routing(Matrix& matrix)
		: I_Algorithm<Matrix>(matrix)
	{
	}

} /* pandr::algorithm namespace */
