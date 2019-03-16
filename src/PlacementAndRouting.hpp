#pragma once
#include <algorithms/I_Algorithm.hpp>
#include <algorithms/I_Placement.hpp>
#include <algorithms/Placements.hpp>
#include <Network.hpp>
#include <Field.hpp>
#include <Types.hpp>
#include <Exception.hpp>
#include <Router.hpp>
#include <json.hpp>

namespace pandr {
	template<typename F, typename N, template<typename T, typename U> typename P>
	class PlacementAndRouting {
		private:
			N const& ntk;
			P<F,N> placements;
			Router<F,N,P> router;
		public:
			PlacementAndRouting(N const& ntk);
			auto run();
	};

	template<typename F, typename N, template<typename T, typename U> typename P>
	PlacementAndRouting<F,N,P>::PlacementAndRouting(N const& ntk)
		: ntk(ntk)
		, placements(ntk)
		, router(ntk, placements)
	{
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	auto PlacementAndRouting<F,N,P>::run() {
		using placement_failure = typename decltype(placements)::placement_failure;
		using zero_placement_failure = typename decltype(placements)::zero_placement_failure;
		using routing_failed = typename Router<F,N,P>::routing_failed;
		using Placements = pandr::algorithm::Placements<F>;
		using Placement = pandr::algorithm::Placement;
		using json = nlohmann::json;


		this->placements.initialDistance();
		auto ntk_depth = this->ntk.depth();


		Placements p(this->placements.getField());
		auto placement_routing = [&]() -> bool {
			while(this->placements.level() != ntk_depth){
				try{
					++this->placements;
					this->router.route();
				}catch(placement_failure const& e){
					this->router.unroute();
					this->placements.blacklist();
					--this->placements;
				}catch(routing_failed const& e){
					this->router.unroute();
					this->placements.blacklist();
					--this->placements;
				}catch(zero_placement_failure const& e){
					return false;
				}
			}
			return true;
		};

		if(placement_routing()){
			std::cout << "\033[1;34m * \033[0mPlacement and Routing Success" << std::endl;
		}else{
			std::cout << "\033[1;31m * \033[0mPlacement and Routing Failure" << std::endl;
		}

		auto routes_level {this->router.get()};

		json j;
		j["Area"] = this->placements.getField().area();

		for(auto const& node : this->ntk.nodes_at_level(0)){
			j["0"][std::to_string(node)] = this->placements.find(node)->current();
		}

		std::for_each(std::begin(routes_level), std::end(routes_level), [&](auto const& entry){
			std::string level {std::to_string(entry.first)};
			for(auto const& route : entry.second){
				std::string fo {std::to_string(std::get<0>(route))};
				std::string fi {std::to_string(std::get<1>(route))};
				auto& path {std::get<2>(route)};
				j[level.c_str()][fo.c_str()][fi.c_str()] = path;
			}
		});

		return j;
	}
} /* pandr namespace */
