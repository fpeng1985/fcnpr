#pragma once
#include <algorithms/I_Algorithm.hpp>
#include <simulated-annealing/I_SimulatedAnnealing.hpp>
#include <limits>
#include <cmath>
#include <unordered_map>
#include <random>
#include <Types.hpp>
#include <algorithm>
#include <algorithms/I_Placement.hpp>
#include <iostream>

namespace pandr::algorithm {
	template<typename Matrix, typename Ntk>
	class SimulatedAnnealing : public meta::sa::I_SimulatedAnnealing<int64_t,Placements<Matrix>>, public I_Placement<Matrix,Ntk> {
		private:
			mutable std::random_device rd;
			mutable std::mt19937 rgn;
		public:
			SimulatedAnnealing(Ntk const& ntk);
			int64_t cost(Placements<Matrix> const& src) const noexcept override final;
			virtual Placements<Matrix> perturb(Placements<Matrix> const& src) noexcept override final;
			virtual void init(Placements<Matrix> const& src) noexcept override final;
			virtual Placements<Matrix> algorithm(Placements<Matrix> const& placements) override final;
	};

	template<typename Matrix, typename Ntk>
	SimulatedAnnealing<Matrix,Ntk>::SimulatedAnnealing(Ntk const& ntk)
		: I_Placement<Matrix,Ntk>(ntk)
		, rgn(rd())
	{
	}

	template<typename Matrix, typename Ntk>
	int64_t SimulatedAnnealing<Matrix,Ntk>::cost(Placements<Matrix> const& src) const noexcept {
		if(this->isBlacklisted(src)) return std::numeric_limits<int64_t>::max();
		return this->matrix.area();
	}

	template<typename Matrix, typename Ntk>
	Placements<Matrix> SimulatedAnnealing<Matrix,Ntk>::perturb(Placements<Matrix> const& src) noexcept {
		Placements<Matrix> result{src};
		std::for_each(result.begin(), result.end(), [&](auto& placement){
			placement = this->randomize(placement);
		});
		return result;
	}

	template<typename Matrix, typename Ntk>
	void SimulatedAnnealing<Matrix,Ntk>::init(Placements<Matrix> const& src) noexcept {
		auto& temperature {this->parameters.current.temperature};
		auto& transitions {this->parameters.current.transitions};

	/* Initial Temperature */
	auto ptemp {src};
	uint32_t itemp{};
	for(auto i{0}; i<200; ++i){
		ptemp = this->perturb(ptemp);
		itemp += this->cost(ptemp);
	}
	itemp /= 200;
	ptemp = src;
	/*Variables*/

		temperature = itemp;
		transitions = 10;

	/*Lambdas*/
		this->parameters.temperature = [&]() -> double {return 0.99*temperature;};
		this->parameters.transition = [&]() -> uint32_t {return transitions;};
		this->parameters.stop = [&](auto const& solution) -> bool {
			static int64_t previous{};
			static int64_t candidate{};
			static uint32_t failures{};

			candidate = this->cost(solution);

			if(candidate >= previous){
				failures++;
			}else{
				failures = 0;
			}

			previous = candidate;

			return (failures > 100 && temperature <= 0.2);
		};
	}

	template<typename Matrix, typename Ntk>
	Placements<Matrix> SimulatedAnnealing<Matrix,Ntk>::algorithm(Placements<Matrix> const& placements) {
		return this->run(placements);
	}

} /* pandr::algorithm namespace */
