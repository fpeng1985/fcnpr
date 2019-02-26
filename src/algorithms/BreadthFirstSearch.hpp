#pragma once
#include <Types.hpp>
#include <Traits.hpp>
#include <queue>
#include <algorithms/I_Routing.hpp>

namespace pandr::algorithm {
	template<typename Matrix>
	class BreadthFirstSearch : public I_Routing<Matrix> {
		public:
			BreadthFirstSearch() = delete;
			BreadthFirstSearch(Matrix& matrix);
			virtual Path run(Region const& src, Region const& dest) const override;
	};

	template<typename Matrix>
	BreadthFirstSearch<Matrix>::BreadthFirstSearch(Matrix& matrix)
		: I_Routing<Matrix>(matrix)
	{
	}

	template<typename Matrix>
	Path BreadthFirstSearch<Matrix>::run(Region const& src, Region const& dest) const {
		using std::vector;
		using std::pair;
		using std::queue;
		using std::get;
		using Cell = pair<uint64_t,uint64_t>;
		using Cells = Path;

		auto const [x1,y1] = src;
		auto const [x2,y2] = dest;

		auto area {this->matrix.size()};
		Path min_path;
		queue<Cell> cell_queue;
		queue<Path> path_queue;
		vector<vector<bool>> visited(area, vector<bool>(area, false)); //Visited Cells

		cell_queue.push({x1,y1});
		path_queue.push({{x1,y1}});
		visited[x1][y1] = true;


		while(!cell_queue.empty()){
			auto curr_cell = cell_queue.front(); cell_queue.pop();
			auto curr_path = path_queue.front(); path_queue.pop();

			auto x = get<0>(curr_cell);
			auto y = get<1>(curr_cell);

			if(x == x2 && y == y2){
				min_path = curr_path;
				break;
			}

			auto move = [&](const decltype(x) x, const decltype(y) y){
				curr_path.push_back({x, y});
				cell_queue.push({x, y});
				path_queue.push(curr_path);
				visited[x][y] = true;
				curr_path.pop_back();

			};

			auto const& cell {this->matrix.at(x,y)};

			//Upwards
			if(x-1 > 0 && visited[x-1][y] == false && cell > this->matrix.at(x-1, y)){
				move(x-1, y);
			}
			//Downwards
			if(x+1 < area && visited[x+1][y] == false && cell > this->matrix.at(x+1, y)){
				move(x+1, y);
			}
			//Right
			if(y+1 < area && visited[x][y+1] == false && cell > this->matrix.at(x, y+1)){
				move(x, y+1);
			}
			//Left
			if(y-1 > 0 && visited[x][y-1] == false && cell > this->matrix.at(x, y-1)){
				move(x, y-1);
			}
		}
		return min_path;
	}

} /* pandr::algorithm namespace */
