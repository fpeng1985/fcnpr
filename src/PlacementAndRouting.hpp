#pragma once
#include <algorithms/I_Algorithm.hpp>
#include <algorithms/I_Placement.hpp>
#include <algorithms/Placements.hpp>
#include <Network.hpp>
#include <Field.hpp>
#include <Types.hpp>
#include <Exception.hpp>
#include <Router.hpp>
#include <PandrJson.hpp>
#include <chrono>

namespace pandr {
	template<typename F, typename N, template<typename T, typename U> typename P>
	class PlacementAndRouting {
		private:
			N const& ntk;
			std::shared_ptr<P<F,N>> placements;
			std::shared_ptr<Router<F,N,P>> router;
			double pandr_duration;
			static bool first_run;
		public:
		/* Constructors */
			PlacementAndRouting(N const& ntk);
			PlacementAndRouting(PlacementAndRouting&& src) = delete;
			PlacementAndRouting(PlacementAndRouting const& src) = delete;
		/* Methods */
			void run();
			double duration() const noexcept;
			pandr::Json json() const noexcept;
	};

	template<typename F, typename N, template<typename T, typename U> typename P>
	bool PlacementAndRouting<F,N,P>::PlacementAndRouting::first_run = true;

	/*
	 * Constructors
	 */
	template<typename F, typename N, template<typename T, typename U> typename P>
	PlacementAndRouting<F,N,P>::PlacementAndRouting(N const& ntk)
		: ntk(ntk)
		, placements(std::make_shared<P<F,N>>(ntk))
		, router(std::make_shared<Router<F,N,P>>(ntk, *placements))
		, pandr_duration(0)
	{
	}

	/*
	 * Methods
	 */
	template<typename F, typename N, template<typename T, typename U> typename P>
	void PlacementAndRouting<F,N,P>::run() {
		using placement_failure = typename P<F,N>::placement_failure;
		using zero_placement_failure = typename P<F,N>::zero_placement_failure;
		using routing_failed = typename Router<F,N,P>::routing_failed;
		using Placements = pandr::algorithm::Placements<F>;

		if(this->first_run){
			this->first_run = false;
		}else{
			this->placements = std::make_shared<P<F,N>>(ntk);
			this->router = std::make_shared<Router<F,N,P>>(ntk, *placements);
			this->pandr_duration = 0;
		}

		this->placements->initialDistance();
		auto ntk_depth = this->ntk.depth();


		Placements p(this->placements->getField());
		auto placement_routing = [&]() -> bool {
			while(this->placements->level() != ntk_depth){
				try{
					++(*(this->placements));
					this->router->route();
				}catch(placement_failure const& e){
					this->router->unroute();
					this->placements->blacklist();
					--(*(this->placements));
				}catch(routing_failed const& e){
					this->router->unroute();
					this->placements->blacklist();
					--(*(this->placements));
				}catch(zero_placement_failure const& e){
					return false;
				}
			}
			return true;
		};

		auto start = std::chrono::steady_clock::now();
		if(placement_routing()){
			std::cout << "\033[1;34m * \033[0mPlacement and Routing Success" << std::endl;
		}else{
			std::cout << "\033[1;31m * \033[0mPlacement and Routing Failure" << std::endl;
		}
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		this->pandr_duration = std::chrono::duration<double, std::milli>(diff).count();
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	double PlacementAndRouting<F,N,P>::duration() const noexcept {
		return this->pandr_duration;
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	pandr::Json PlacementAndRouting<F,N,P>::json() const noexcept {
		pandr::Json j;

		auto routes_level {this->router->get()};

		j["pandr"]["area"] = this->placements->getField().area();

		for(auto const& node : this->ntk.nodes_at_level(0)){
			j["pandr"]["0"][std::to_string(node)] = this->placements->find(node)->current();
		}

		std::for_each(std::begin(routes_level), std::end(routes_level), [&](auto const& entry){
			std::string level {std::to_string(entry.first)};
			for(auto const& route : entry.second){
				auto& path {std::get<2>(route)};
				std::string fo_str {std::to_string(std::get<0>(route))};
				std::string fi_str {std::to_string(std::get<1>(route))};
				j["pandr"][level.c_str()][fo_str.c_str()][fi_str.c_str()]["route"] = path;
			}
		});

		return j;
	}
} /* pandr namespace */
