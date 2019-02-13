#pragma once
#include <Traits.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class Algorithm {
		protected:
			Matrix const& matrix;
		public:
			Algorithm() = delete;
			Algorithm(Matrix const& matrix);
			virtual ~Algorithm() = default;
	};

	template<typename Matrix>
	Algorithm<Matrix>::Algorithm(Matrix const& matrix)
		: matrix(matrix)
	{
		/*
		 * Static Assetions
		 */
		static_assert(pandr::traits::size<Matrix>::value, "Error: The matrix type does not support the 'size' method");
	}
} /* pandr::algorithm namespace */
