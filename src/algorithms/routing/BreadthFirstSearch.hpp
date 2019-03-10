#pragma once
#include <algorithms/I_Algorithm.hpp>
#include <Types.hpp>
#include <Traits.hpp>
#include <queue>

namespace pandr::algorithm {
	template<typename Matrix>
	class BreadthFirstSearch {
		private:
			Matrix const& matrix;
		public:
			BreadthFirstSearch(Matrix& matrix);
			Route run(Region const& src, Region const& dest) const;
	};

	template<typename Matrix>
	BreadthFirstSearch<Matrix>::BreadthFirstSearch(Matrix& matrix)
		:matrix(matrix)
	{
	}

	template<typename Matrix>
	Route BreadthFirstSearch<Matrix>::run(Region const& src, Region const& dest) const {
		using std::vector;
		using std::pair;
		using std::queue;
		using std::get;

		auto const [x1,y1] = src;
		auto const [x2,y2] = dest;

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

			auto move = [&](const decltype(x) x, const decltype(y) y){
				curr_route.push_back({x, y});
				region_queue.push({x, y});
				route_queue.push(curr_route);
				visited[x][y] = true;
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

} /* pandr::algorithm namespace */
