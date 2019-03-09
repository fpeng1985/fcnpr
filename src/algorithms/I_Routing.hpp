#pragma once
#include <Types.hpp>
#include <algorithms/I_Algorithm.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class I_Routing : public I_Algorithm<Matrix> {
		public:
			I_Routing();
			virtual ~I_Routing() = default;
			virtual Route run(Region const& src, Region const& dest) const = 0;
	};

	template<typename Matrix>
	I_Routing<Matrix>::I_Routing(){
	}

} /* pandr::algorithm namespace */
