#pragma once
#include <Types.hpp>

namespace pandr {
	using FoFiRoute = std::tuple<uint64_t,uint64_t,Route>;

	template<typename F, typename N, template<typename T, typename U> typename P>
	class Router {
		private:
			std::map<uint32_t, std::vector<FoFiRoute>> routes_level;
			P<F,N>& placements;
			F& field;
			N const& ntk;
		public:
			Router(N const& ntk, P<F,N>& placements);
			void route();
			void unroute();
			std::map<uint32_t, std::vector<FoFiRoute>> const& get() const noexcept;
			class routing_failed : public pandr::exception {using exception::exception;};
	};

	template<typename F, typename N, template<typename T, typename U> typename P>
	Router<F,N,P>::Router(N const& ntk, P<F,N>& placements)
		: placements(placements)
		, field(placements.getField())
		, ntk(ntk)
	{
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	void Router<F,N,P>::route() {
		std::vector<FoFiRoute> routes_cache;
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
				if(!this->field.setWire(curr_path)){
					throw routing_failed("Failed to set wire due to region factor limit being reached");
				}
				routes_cache.push_back({fo_id, fanin, curr_path});
			}
		}
		this->routes_level[this->placements.level()] = routes_cache;
	}

	template<typename F, typename N, template<typename T, typename U> typename P>
	void Router<F,N,P>::unroute() {
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
	std::map<uint32_t, std::vector<FoFiRoute>> const& Router<F,N,P>::get() const noexcept {
		return this->routes_level;
	}
} /* pandr namespace */
