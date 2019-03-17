#pragma once
#include <utility>
#include <algorithms/I_Algorithm.hpp>
#include <Types.hpp>
#include <Traits.hpp>
#include <Exception.hpp>
#include <queue>

namespace pandr::algorithm {
	using Cache = std::map<Region, std::map<Region, Route>>;

	template<typename Matrix>
	class DynamicProgramming {
		private:
			static Cache cache;
			static bool is_cached;
		public:
			DynamicProgramming() = default;
			Route run(Matrix const& matrix, Region const& src, Region const& dest);
		private:
			void BuildCache(Matrix const& matrix);
			Route algorithm(Matrix const& matrix, Cache& cache, Region const& src, Region const& dest);
		/*Exceptions*/
			class inconsistent_cache : public pandr::exception {using exception::exception;};

	};

	template<typename Matrix>
	bool DynamicProgramming<Matrix>::is_cached = false;

	template<typename Matrix>
	Cache DynamicProgramming<Matrix>::DynamicProgramming::cache;

	template<typename Matrix>
	void DynamicProgramming<Matrix>::BuildCache(Matrix const& matrix) {
		std::cout << "\033[34;1m * \033[mCaching Dynamic Programming data structures..." << std::endl;
		Cache tmp_cache;

		auto sz {matrix.size()};
		for(auto i{1}; i<sz; ++i){
			for(auto j{1}; j<sz; ++j){
				std::map<Region, Route> tmp;
				tmp_cache.insert({{i,j}, tmp});
			}
		}

		for(auto i{1}; i<sz; ++i){
			for(auto j{1}; j<sz; ++j){
				Region src {i,j};
				for(auto k{sz-1}; k > 0; --k){
					for(auto l{sz-1}; l > 0; --l){
						Region dest {k,l};
						if(src == dest) continue;
						tmp_cache.at(src).insert({dest, this->algorithm(matrix, tmp_cache,src,dest)});
					}
				}
			}
		}
		this->cache = tmp_cache;
	}

	template<typename Matrix>
	Route DynamicProgramming<Matrix>::algorithm(Matrix const& matrix, Cache& c, Region const& src, Region const& dest) {
		using std::vector;
		using std::pair;
		using std::queue;
		using std::get;

		auto const [x1,y1] = src;
		auto const [x2,y2] = dest;

		auto src_search = c.find(src);
		if(src_search != std::end(c)){
			auto const& dest_map {src_search->second};
			auto dest_search = dest_map.find(dest);
			if(dest_search != std::end(dest_map)){
				return dest_search->second;
			}
		}

		auto size {matrix.size()};
		Route min_route;
		queue<Region> region_queue;
		queue<Route> route_queue;
		vector<vector<bool>> visited(size, vector<bool>(size, false)); //Visited Regions

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
				c.at(src).insert({dest, route});
			};

			auto move = [&](const decltype(x) x, const decltype(y) y){
				curr_route.push_back({x, y});
				region_queue.push({x, y});
				route_queue.push(curr_route);
				visited[x][y] = true;
				update_cache({x,y}, curr_route);
				curr_route.pop_back();
			};

			auto const& region {matrix.at(x,y)};

			//Upwards
			if(x-1 > 0 && visited[x-1][y] == false && region > matrix.at(x-1, y)){
				move(x-1, y);
			}
			//Downwards
			if(x+1 < size && visited[x+1][y] == false && region > matrix.at(x+1, y)){
				move(x+1, y);
			}
			//Right
			if(y+1 < size && visited[x][y+1] == false && region > matrix.at(x, y+1)){
				move(x, y+1);
			}
			//Left
			if(y-1 > 0 && visited[x][y-1] == false && region > matrix.at(x, y-1)){
				move(x, y-1);
			}
		}
		return min_route;
	}

	template<typename Matrix>
	Route DynamicProgramming<Matrix>::run(Matrix const& matrix, Region const& src, Region const& dest) {
		if(!this->is_cached){
			BuildCache(matrix);
			this->is_cached = true;
		}
		if(src == dest) return Route();
		return this->cache.at(src).at(dest);
	}

} /* pandr::algorithm namespace */
