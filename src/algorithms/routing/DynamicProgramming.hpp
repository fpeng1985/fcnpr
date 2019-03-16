#pragma once
#include <utility>
#include <algorithms/I_Algorithm.hpp>
#include <Types.hpp>
#include <Traits.hpp>
#include <Exception.hpp>
#include <queue>

namespace pandr::algorithm {
	template<typename Matrix>
	class DynamicProgramming {
		private:
			Matrix& matrix;
			std::map<Region, std::map<Region, Route>> cache;
			bool is_cached;
		public:
			DynamicProgramming(Matrix& matrix);
			Route run(Region const& src, Region const& dest);
		private:
			Route find(Region const& src, Region const& dest);
			void BuildCache();
			Route algorithm(Region const& src, Region const& dest);
		/*Exceptions*/
			class inconsistent_cache : public pandr::exception {using exception::exception;};

	};

	template<typename Matrix>
	DynamicProgramming<Matrix>::DynamicProgramming(Matrix& matrix)
		: matrix(matrix)
		, is_cached(false)
	{
		for(auto i{1}; i<this->matrix.size(); ++i){
			for(auto j{1}; j<this->matrix.size(); ++j){
				std::map<Region, Route> tmp;
				this->cache.insert({{i,j}, tmp});
			}
		}
	}

	template<typename Matrix>
	Route DynamicProgramming<Matrix>::find(Region const& src, Region const& dest) {
		auto src_search = this->cache.find(src);
		if(src_search == std::end(this->cache)) throw inconsistent_cache("Inconsistent Dynamic Programming cache");

		auto const& dest_map {src_search->second};
		return dest_map.find(dest);
	}

	template<typename Matrix>
	void DynamicProgramming<Matrix>::BuildCache() {
		std::cout << "\033[34;1m * \033[mCaching Dynamic Programming data structures..." << std::endl;
		for(auto i{1}; i<matrix.size(); ++i){
			for(auto j{1}; j<matrix.size(); ++j){
				Region src {i,j};
				for(auto k{this->matrix.size()-1}; k > 0; --k){
					for(auto l{this->matrix.size()-1}; l > 0; --l){
						Region dest {k,l};
						if(src == dest) continue;
						this->cache.at(src).insert({dest, this->algorithm(src,dest)});
					}
				}
			}
		}
	}

	template<typename Matrix>
	Route DynamicProgramming<Matrix>::algorithm(Region const& src, Region const& dest) {
		using std::vector;
		using std::pair;
		using std::queue;
		using std::get;

		auto const [x1,y1] = src;
		auto const [x2,y2] = dest;

		auto src_search = this->cache.find(src);
		if(src_search != std::end(this->cache)){
			auto const& dest_map {src_search->second};
			auto dest_search = dest_map.find(dest);
			if(dest_search != std::end(dest_map)){
				return dest_search->second;
			}
		}

		auto area {this->matrix.size()};
		Route min_route;
		queue<Region> region_queue;
		queue<Route> route_queue;
		vector<vector<bool>> visited(area, vector<bool>(area, false)); //Visited Regions

		region_queue.push({x1,y1});
		route_queue.push({{x1,y1}});
		visited[x1][y1] = true;


		while(!region_queue.empty()){
			auto curr_region = region_queue.front(); region_queue.pop();
			auto curr_route = route_queue.front(); route_queue.pop();

			auto x = get<0>(curr_region);
			auto y = get<1>(curr_region);

			if(x == x2 && y == y2){
				min_route = curr_route;
				break;
			}

			auto update_cache = [&](Region const& dest, Route const& route){
				this->cache.at(src).insert({dest, route});
			};

			auto move = [&](const decltype(x) x, const decltype(y) y){
				curr_route.push_back({x, y});
				region_queue.push({x, y});
				route_queue.push(curr_route);
				visited[x][y] = true;
				update_cache({x,y}, curr_route);
				curr_route.pop_back();
			};

			auto const& region {this->matrix.at(x,y)};

			//Upwards
			if(x-1 > 0 && visited[x-1][y] == false && region > this->matrix.at(x-1, y)){
				move(x-1, y);
			}
			//Downwards
			if(x+1 < area && visited[x+1][y] == false && region > this->matrix.at(x+1, y)){
				move(x+1, y);
			}
			//Right
			if(y+1 < area && visited[x][y+1] == false && region > this->matrix.at(x, y+1)){
				move(x, y+1);
			}
			//Left
			if(y-1 > 0 && visited[x][y-1] == false && region > this->matrix.at(x, y-1)){
				move(x, y-1);
			}
		}
		return min_route;
	}

	template<typename Matrix>
	Route DynamicProgramming<Matrix>::run(Region const& src, Region const& dest) {
		if(src == dest) return Route();
		if(this->is_cached){
			return this->cache.at(src).at(dest);
		}else{
			this->BuildCache();
			this->is_cached = true;
			return this->cache.at(src).at(dest);
		}
	}

} /* pandr::algorithm namespace */
