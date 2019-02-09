#pragma once
#include <Types.hpp>
#include <Traits.hpp>

namespace pandr::algorithm {
	//The BFS algorithm is used to find the shortest distance between the given points

	template<typename Matrix>
	Path breadthFirstSearch(const Matrix& matrix, uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2) {

		static_assert(pandr::traits::get<Matrix>::value, "Error: The matrix type does not support the 'get' method");
		static_assert(pandr::traits::size<Matrix>::value, "Error: The matrix type does not support the 'size' method");

		using std::vector;
		using std::pair;
		using std::queue;
		using std::get;
		using Cell = pair<uint64_t,uint64_t>;
		using Cells = Path;

		auto area {matrix.size()};
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

			auto cell {matrix.get(x,y)};

			//Upwards
			if(x-1 > 0 && visited[x-1][y] == false && matrix.get(x-1, y) > cell){
				move(x-1, y);
			}
			//Downwards
			if(x+1 < area && visited[x+1][y] == false && matrix.get(x+1, y) > cell){
				move(x+1, y);
			}
			//Right
			if(y+1 < area && visited[x][y+1] == false && matrix.get(x, y+1) > cell){
				move(x, y+1);
			}
			//Left
			if(y-1 > 0 && visited[x][y-1] == false && matrix.get(x, y-1) > cell){
				move(x, y-1);
			}
		}
		return min_path;
	}

} /* pandr::algorithm namespace */
