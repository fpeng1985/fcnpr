#pragma once
#include <Types.hpp>
#include <algorithms/I_Algorithm.hpp>

namespace pandr::algorithm {
	template<typename Matrix, typename Ntk>
	class I_Placement : public I_Algorithm<Matrix> {
		protected:
			Ntk const& ntk;
		public:
			I_Placement() = delete;
			I_Placement(Matrix const& matrix, Ntk const& ntk);
			virtual Placements run() const = 0;
			virtual ~I_Placement() = default;
	};

	template<typename Matrix, typename Ntk>
	I_Placement<Matrix, Ntk>::I_Placement(Matrix const& matrix, Ntk const& ntk)
		: I_Algorithm<Matrix>(matrix)
		, ntk(ntk)
	{
	}

} /* pandr::algorithm namespace */
