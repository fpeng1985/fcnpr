#pragma once
#include <Types.hpp>
#include <algorithms/I_Algorithm.hpp>

namespace pandr::algorithm {
	template<typename Matrix, typename Ntk>
	class Placement : public Algorithm<Matrix> {
		private:
			Ntk const& ntk;
		public:
			Placement() = delete;
			Placement(Matrix const& matrix, Ntk const& ntk);
			virtual Slots run() const = 0;
			virtual ~Placement() = default;
	};

	template<typename Matrix, typename Ntk>
	Placement<Matrix, Ntk>::Placement(Matrix const& matrix, Ntk const& ntk)
		: Algorithm<Matrix>(matrix)
		, ntk(ntk)
	{
	}

} /* pandr::algorithm namespace */
