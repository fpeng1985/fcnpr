#pragma once
#include <algorithms/I_Algorithm.hpp>
#include <algorithms/I_Placement.hpp>
#include <algorithms/I_Routing.hpp>
#include <algorithms/Placements.hpp>
#include <Network.hpp>
#include <Field.hpp>
#include <Types.hpp>
#include <Exception.hpp>

namespace pandr {
	template<typename F, typename N, template<typename T, typename U> typename P>
	class PlacementAndRouting {
		private:
			N ntk;
			P<F,N> placements;
			F& field;
		public:
			PlacementAndRouting();
			Routes route();
			void unroute();
			auto run();
			class routing_failed : public pandr::exception {using exception::exception;};
	};

	template<typename F, typename N, template<typename T, typename U> typename P>
	PlacementAndRouting<F,N,P>::PlacementAndRouting()
		: ntk(4)
		, placements(ntk)
		, field(placements.getField())
	{
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	Routes PlacementAndRouting<F,N,P>::route() {
		Routes routes_cache;
		if(this->placements.level() <= 0) return routes_cache;
		auto fo{this->placements.current()};
		for(auto it{fo.cbegin()}; it!=fo.cend(); ++it){
			auto fo {*it};
			auto fo_id = fo.id();
			auto [fo_x,fo_y] = fo.current();
			auto fanins {this->ntk.node_fan_ins(fo_id)};
			for(auto fanin : fanins){
				auto placement {this->placements.find(fanin)};
				auto [fi_x,fi_y] = placement->current();
				auto curr_path {this->field.getRelativeMinRoute(fi_x,fi_y,fo_x,fo_y)};
				if(!this->field.setWire(curr_path)){
					for(auto const& route : routes_cache){
						this->field.unsetWire(route);
					}
					throw routing_failed("Failed to set wire due to region factor limit being reached");
				}
				routes_cache.push_back(curr_path);
			}
		}
		return routes_cache;
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	void PlacementAndRouting<F,N,P>::unroute() {
		if(this->placements.level() <= 0) return;
		auto fo{this->placements.current()};
		for(auto it{fo.cbegin()}; it!=fo.cend(); ++it){
			auto fo {*it};
			auto fo_id = fo.id();
			auto [fo_x,fo_y] = fo.current();
			auto fanins {this->ntk.node_fan_ins(fo_id)};
			for(auto fanin : fanins){
				auto placement {this->placements.find(fanin)};
				auto [fi_x,fi_y] = placement->current();
				auto curr_path {this->field.getRelativeMinRoute(fi_x,fi_y,fo_x,fo_y)};
				this->field.unsetWire(curr_path);
			}
		}
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	auto PlacementAndRouting<F,N,P>::run() {
		using placement_failure = typename decltype(placements)::placement_failure;
		using zero_placement_failure = typename decltype(placements)::zero_placement_failure;
		using Placements = pandr::algorithm::Placements<F>;
		using Placement = pandr::algorithm::Placement;

		lorina::read_verilog("C17.v", mockturtle::verilog_reader(this->ntk));
		this->placements.initialDistance();
		auto ntk_depth = this->ntk.depth();


		std::cout << this->ntk << std::endl;

		Placements p(this->field);
		auto placement_routing = [&]() -> bool {
			while(this->placements.level() != ntk_depth){
				try{
					++this->placements;
					this->route();
				}catch(placement_failure const& e){
					this->unroute();
					this->placements.blacklist();
					--this->placements;
				}catch(routing_failed const& e){
					this->unroute();
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

		return this->field;
	}
} /* pandr namespace */
