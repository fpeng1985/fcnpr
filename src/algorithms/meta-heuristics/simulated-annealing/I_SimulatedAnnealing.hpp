#pragma once
#include <functional>
#include <limits>
#include <cmath>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <iostream>
#include "Parameters.hpp"

namespace meta::sa {
	template<typename C, typename S>
	class I_SimulatedAnnealing {
		public:
			Parameters<C,S> parameters;
		protected:
			mutable std::random_device rd;
			mutable std::mt19937 rgn;
		protected:
			virtual C cost(S const& s) const noexcept = 0;
			virtual S perturb(S const& s) noexcept = 0;
			virtual void init(S const& s) noexcept = 0;
		public:
			I_SimulatedAnnealing();
			S run(S const& s);
	};

	template<typename C, typename S>
	I_SimulatedAnnealing<C,S>::I_SimulatedAnnealing()
		: rgn(rd())
	{
	}

	template<typename C, typename S>
	S I_SimulatedAnnealing<C,S>::run(S const& s) {
		std::uniform_real_distribution<double> r_dist(0.0, 1.0);

		auto& transitions = this->parameters.current.transitions;
		auto& temperature = this->parameters.current.temperature;
		auto solution {s};

		this->init(solution);

		decltype(this->cost(solution)) old_cost;
		decltype(old_cost) new_cost;

		do {
			auto cost = this->cost(solution);
			for(auto i{0}; i<transitions; ++i){
				S old_s{solution};

				old_cost = this->cost(solution);

				solution = this->perturb(solution);

				new_cost = this->cost(solution);

				if(new_cost <= old_cost){
					continue;
				}else{
					auto prob {std::exp((old_cost-new_cost)/temperature)};
					auto random_double {r_dist(rgn)};
					if(prob >= random_double){
						continue;
					}else{
						solution = old_s;
					}
				}
			}
			temperature = this->parameters.temperature();
			transitions = this->parameters.transition();
		}while(!this->parameters.stop(solution));

		if(temperature > 0.2){
			std::clog << "\033[1;31m * \033[0mFinished simulated annealing algorithm temperature still high of " << temperature << std::endl;
		}

		auto prob = std::exp((old_cost-new_cost)/temperature) > 0.1;
		if(prob > 0.2){
			std::clog << "\033[1;31m * \033[0mFinished simulated annealing algorithm high probability of accepting bad solutions, current is around " << prob << std::endl;
		}

		return solution;
	}

} /* algorithm namespace */
