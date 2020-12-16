#include <vector>
#include <utility>
#include "./classes/step.h"
#include <set>
#include "./classes/Chebyshev.h"
using namespace std;

namespace astarche
{
	class FComparison {
	public:
		bool operator()(const Step& a, const Step& b) const {
			if (a.get_total_cost() == b.get_total_cost()) return a.get_coordinate_code() < b.get_coordinate_code();
			else return a.get_total_cost() < b.get_total_cost();
		}
	};

	class Solution {
	private:
		
		const int dx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
		const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		vector<vector<int> > maze;
		pair<int, int> size;
		pair<int, int> start;
		pair<int, int> finish;
		int min_cost = 0x7fffffff;

		vector<Step> routes;
		vector<Step> process;

		Step* parent;
		bool* open;
		bool* closed;
		set<Step, FComparison> open_list;
		vector<int> close_list;
	public:
		Solution() {}
		Solution(const pair<int, int>, const pair<int, int>, const vector<vector<int> >, const pair<int, int>);

		int get_min_cost();
		vector<Step>& get_routes();
		vector<Step>& get_process();

		template <class T> void Astar();
		bool check(const pair<int, int>&, const pair<int, int>&, const bool&);
		void find_routes(const Step&);
	};

	Solution::Solution(const pair<int, int> start, const pair<int, int> finish, const vector<vector<int> > maze, const pair<int, int> size)
		: start(start), finish(finish), maze(maze), size(size)
	{
		const int num = size.first * size.second + 1;
		this->parent = new Step[num];
		this->open = new bool[num];
		this->closed = new bool[num];

		memset(open, false, num * sizeof(bool));
		memset(closed, false, num * sizeof(bool));
	}

	int Solution::get_min_cost() {
		return this->min_cost;
	}

	vector<Step>& Solution::get_routes() {
		return this->routes;
	}

	vector<Step>& Solution::get_process() {
		return this->process;
	}

	template <class T>
	void Solution::Astar() {
		open_list.insert(Step(Step::convert_to_code(start, size.second), 1));
		process.push_back(Step(Step::convert_to_code(start, size.second), 1));
		open[Step::convert_to_code(start, size.second)] = true;
		parent[Step::convert_to_code(start, size.second)] = Step(-1);
		while (!open_list.empty()) {
			Step now = *open_list.begin();
			int now_code = now.get_coordinate_code();
			int nowx = now.get_coordinate(size.second).first;
			int nowy = now.get_coordinate(size.second).second;
			open_list.erase(open_list.begin());
			open[now_code] = false;
			if (now_code == Step::convert_to_code(finish, size.second)) {
				find_routes(now);
				return;
			}

			close_list.push_back(now_code);
			closed[now_code] = true;

			for (int i = 0; i < 8; i++) {
				int x = nowx + dx[i], y = nowy + dy[i];
				int new_code = Step::convert_to_code(make_pair(x, y), size.second);
				bool diagonal = false;
				if (dx[i] && dy[i]) diagonal = true;
				if (!check(make_pair(x, y), make_pair(dx[i], dy[i]), diagonal)) continue;
				if (!open[new_code]) {
					Step new_step = Step(new_code, now.get_time() + 1, (now.get_actual_cost() == -1 ? 0 : now.get_actual_cost()) + (diagonal ? 14 : 10), T::get_new_estimate_cost(make_pair(x, y), finish));
					parent[new_code] = now;
					open_list.insert(new_step);
					open[new_code] = true;
					process.push_back(new_step);
				}
			}
		}
	}

	bool Solution::check(const pair<int, int>& coordinate, const pair<int, int>& direction, const bool& diagonal) {
		int x = coordinate.first, y = coordinate.second;
		int height = this->size.first, width = this->size.second;

		int dx = direction.first, dy = direction.second;

		
		if (x < 0 || y < 0 || x >= height || y >= width) return false;
		
		if (this->maze[x][y]) return false;
		
		if (diagonal && this->maze[x - dx][y] && this->maze[x][y - dy]) return false;
		
		if (this->closed[Step::convert_to_code(coordinate, width)]) return false;

		return true;
	}

	void Solution::find_routes(const Step& step) {
		if (step.get_coordinate_code() == -1) return;
		find_routes(parent[step.get_coordinate_code()]);
		routes.push_back(step);
	}

	Solution solve(
		const pair<int, int>& start,
		const pair<int, int>& finish,
		const vector<vector<int> >& maze,
		const pair<int, int>& size)
	{
		Solution solution(start, finish, maze, size);

		solution.Astar<Chebyshev>();

		return solution;
	}
}