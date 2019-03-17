#pragma once
#include <algorithms/I_Algorithm.hpp>
#include <algorithms/Placement.hpp>
#include <algorithms/Placements.hpp>
#include <algorithms/Blacklist.hpp>
#include <Cell.hpp>
#include <Exception.hpp>
#include <Types.hpp>
#include <memory>
#include <unordered_map>

namespace pandr::algorithm {
	template<typename Matrix, typename Ntk>
	class I_Placement : public I_Algorithm<Matrix> {
		protected:
			std::unordered_map<int32_t, Placements<Matrix>> level_placements;
			Blacklist<Matrix> bls;
			Ntk const& ntk;
			uint64_t initial_distance;
			uint64_t level_multiplier;
			int32_t curr_level;
		public:
		/* Constructors */
			I_Placement() = delete;
			I_Placement(Ntk const& ntk) noexcept;
		/* Destructors */
			virtual ~I_Placement() = default;
		private:
		/* Methods */
			Placements<Matrix> generate(std::vector<uint64_t> nodes);
			void place();
			void unplace();
		protected:
			Placement randomize(Placement const& src) noexcept;
			virtual Placements<Matrix> algorithm(Placements<Matrix> const& placements) = 0;
		public:
			void blacklist() noexcept;
			bool isBlacklisted(Placements<Matrix> const& src) const noexcept;
			placement_const_iterator find(uint64_t identifier) const noexcept;
			void initialDistance();
			int32_t level() const noexcept;
			Placements<Matrix> const& current();
		/* Operators */
			Placements<Matrix> const& operator++();
			Placements<Matrix> const& operator--();
		/* Exceptions */
			class empty_graph : public pandr::exception {using exception::exception;};
			class placement_failure : public pandr::exception{using exception::exception;};
			class invalid_initial_distance : public pandr::exception{using exception::exception;};
			class zero_placement_failure : public pandr::exception{using exception::exception;};
		/* Debug TODO remove */
			Matrix& getField() const noexcept;
	};

	template<typename Matrix>
	Matrix I_Algorithm<Matrix>::matrix{Matrix()};

	/*
	 * Constructors
	 */
	template<typename Matrix, typename Ntk>
	I_Placement<Matrix, Ntk>::I_Placement(Ntk const& ntk) noexcept
		: ntk(ntk)
		, initial_distance(0)
		, level_multiplier(4)
		, curr_level(-1)
	{
	}

	/*
	 * Methods
	 */
	template<typename Matrix, typename Ntk>
	Placements<Matrix> I_Placement<Matrix,Ntk>::generate(std::vector<uint64_t> nodes) {
		auto available_regions = [&](std::pair<uint64_t,uint64_t> pos, uint64_t dist) -> Regions {
			auto regions {this->matrix.neighbors(pos.first, pos.second, dist)};
			if(regions.empty()) throw placement_failure("Empty regions list");
			return std::move(regions);
		};

		auto get_regions = [&](auto const node) -> Regions {

			if(this->ntk.node_level(node) == 0){
				auto const regions {available_regions({this->matrix.size()/2, this->matrix.size()/2}, this->initial_distance)};
				return regions;
			}


			auto fanins {this->ntk.node_fan_ins(node)};
			std::map<Region,uint32_t> count_region;
			std::for_each(std::cbegin(fanins), std::cend(fanins), [&](auto const& fanin){
				auto const distance {(this->ntk.level_distance(node, fanin)*this->level_multiplier)};
				auto const regions {available_regions(this->find(fanin)->current(), distance)};
				std::for_each(std::cbegin(regions), std::cend(regions), [&](auto const& region){
					count_region[region]++;
				});
			});
				
			Regions regions;
			std::for_each(std::begin(count_region), std::end(count_region), [&](auto const& entry){
				if(entry.second == fanins.size()){
					regions.push_back(entry.first);
				}
			});

			if(regions.empty()) throw placement_failure("Empty regions list");

			return regions;
		};

		auto roll_back = [&](auto& ps) -> void {
			for(auto& p : ps){
				this->matrix.unplace(p.current().first, p.current().second);
			}
		};

		Placements<Matrix> ps(this->matrix);
		try{
			for(auto const& node : nodes){
				if(!ps.add(get_regions(node), node)){
					throw placement_failure("All regions occupied");
				}
			}
		}catch(placement_failure const& e){
			roll_back(ps);
			throw e;
		}
		return ps;
	}

	template<typename Matrix, typename Ntk>
	void I_Placement<Matrix, Ntk>::place() {
		try{
			++this->curr_level;
			auto const nodes {this->ntk.nodes_at_level(this->curr_level)};
			auto ps {std::move(this->generate(nodes))};
			Placements<Matrix> tmp(this->matrix);
			tmp = ps;
			ps = this->algorithm(ps);
			if(this->isBlacklisted(ps)){
				std::for_each(ps.cbegin(), ps.cend(), [&](auto const& p){
					this->matrix.unplace(p.current().first, p.current().second);
				});
				throw placement_failure("All the placements in level are blacklisted");
			}
			this->level_placements.insert({this->curr_level, Placements<Matrix>(ps)});
		}catch(placement_failure const& e){
			--this->curr_level;
			throw e;
		}
	}

	template<typename Matrix, typename Ntk>
	void I_Placement<Matrix, Ntk>::unplace() {
		std::for_each(this->current().cbegin(), this->current().cend(), [&](auto const& p){
			this->matrix.unplace(p.current().first,p.current().second);
		});
		this->level_placements.erase(this->curr_level);
		--this->curr_level;
	}

	template<typename Matrix, typename Ntk>
	Placement I_Placement<Matrix,Ntk>::randomize(Placement const& src) noexcept {
		Placement result{src};
		auto id {result.id()};
		auto [x2,y2] = result.random();

		if(this->matrix.place(x2,y2,id)){
			auto [x1,y1] = result.current();
			this->matrix.unplace(x1,y1);
			result.update({x2,y2});
		}

		return result;
	}

	template<typename Matrix, typename Ntk>
	void I_Placement<Matrix, Ntk>::blacklist() noexcept {
		this->bls.add(this->current());
	}

	template<typename Matrix, typename Ntk>
	bool I_Placement<Matrix, Ntk>::isBlacklisted(Placements<Matrix> const& src) const noexcept {
		return this->bls.contains(src);
	}


	template<typename Matrix, typename Ntk>
	placement_const_iterator I_Placement<Matrix, Ntk>::find(uint64_t identifier) const noexcept {
		placement_const_iterator it_level;

		auto it_map = std::find_if(std::cbegin(this->level_placements), std::cend(this->level_placements), [&](auto& map_entry){
				it_level = map_entry.second.find(identifier);
				return (it_level != map_entry.second.cend());
		});

		return it_level;
	}

	template<typename Matrix, typename Ntk>
	void I_Placement<Matrix,Ntk>::initialDistance() {
		try {
			auto initial_nodes {this->ntk.nodes_at_level(0)};
			auto middle {this->matrix.size()/2};
			for(initial_distance=1; initial_nodes.size() > this->matrix.neighbors(middle,middle,initial_distance).size(); ++initial_distance);
		}catch(std::range_error const& e){
			throw empty_graph("You must initialize the graph before calculating the initial distance");
		}
	}

	template<typename Matrix, typename Ntk>
	int32_t I_Placement<Matrix,Ntk>::level() const noexcept {
		return this->curr_level;
	}

	template<typename Matrix, typename Ntk>
	Placements<Matrix> const& I_Placement<Matrix,Ntk>::current() {
		return this->level_placements.at(this->curr_level);
	}

	/*
	 * Operators
	 */
	template<typename Matrix, typename Ntk>
	Placements<Matrix> const& I_Placement<Matrix,Ntk>::operator++() {
		if(initial_distance == 0) throw invalid_initial_distance("You must calculate the initial distance before running the algorithm");
		this->place();
		return this->current();
	}

	template<typename Matrix, typename Ntk>
	Placements<Matrix> const& I_Placement<Matrix,Ntk>::operator--() {
		if(this->level() == 0){
			try{
				this->unplace();
				this->place();
				return this->current();
			}catch(placement_failure const& e){
				throw zero_placement_failure("All level zero placements were blacklisted");
			}
		}else{
			this->unplace();
			return this->current();
		}
	}

	template<typename Matrix, typename Ntk>
	Matrix& I_Placement<Matrix,Ntk>::getField() const noexcept {
		return this->matrix;
	}

} /* pandr::algorithm namespace */
