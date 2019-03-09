#pragma once
#include <Traits.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class I_Algorithm {
		protected:
			static Matrix matrix;
		public:
			I_Algorithm();
			virtual ~I_Algorithm() = default;
	};

	template<typename Matrix>
	I_Algorithm<Matrix>::I_Algorithm(){
		/*
		 * Static Assetions
		 */
		static_assert(pandr::traits::size<Matrix>::value, "Error: The matrix type does not support the 'size' method");
	}
} /* pandr::algorithm namespace */
