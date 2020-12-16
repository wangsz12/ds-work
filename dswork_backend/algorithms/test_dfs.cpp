/***************************************************************
 * @file       test_DFS
 * @brief      DFS
 * @author     wangsz12
 * @version    1.2
 * @date       2020.11.18
 **************************************************************/

#include <iostream>
#include <vector>
#include <utility>
#include "./classes/step.h"
using namespace std;

namespace Dfs {
  class Solution {
    private:
      
      const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
      const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
      vector<vector<int> > maze;
      pair<int, int> size;
      pair<int, int> start;
      pair<int, int> finish;
      vector<vector<bool> > vis;
      int min_cost = 0x7fffffff;

      vector<Step> routes;
      vector<Step> process;
    public:
      Solution() {}
      Solution(const pair<int, int>, const pair<int, int>, const vector<vector<int> >, const pair<int, int>);

      int get_min_cost();
      vector<Step>& get_routes();
      vector<Step>& get_process();

      
      void dfs(const pair<int, int>&, int, vector<Step>, int);
      
      bool check(const pair<int, int>&, const pair<int, int>&);
  };

  Solution::Solution(const pair<int, int> start, const pair<int, int> finish, const vector<vector<int> > maze, const pair<int, int> size)
    : start(start), finish(finish), maze(maze), size(size)
  {
    this->vis.resize(size.first);
    for (vector<vector<bool> >::iterator i = this->vis.begin(); i != this->vis.end(); ++i) {
      for (int j = 0; j < size.second; ++j) {
        (*i).push_back(false);
      }
    }
  }

  int Solution::get_min_cost() {
    return this->min_cost;
  }

  vector<Step>& Solution::get_routes() {
    return this->routes;
  }

  std::vector<Step>& Solution::get_process() {
    return this->process;
  }

  void Solution::dfs(const pair<int, int>& now, int cost, vector<Step> routes, int time) {
    int nowx = now.first, nowy = now.second;
    this->vis[nowx][nowy] = true;
    routes.push_back(Step(Step::convert_to_code(make_pair(nowx, nowy), this->size.second), time, cost));
    process.push_back(Step(Step::convert_to_code(make_pair(nowx, nowy), this->size.second), time, cost));
    if (nowx == this->finish.first && nowy == this->finish.second) {
      if (cost < this->min_cost) {
        this->min_cost = cost;
        this->routes = routes;
      }
      return;
    }
    
    if (cost > this->min_cost) return;
    for (int i = 0; i < 8; i++) {
      int x = nowx + dx[i];
      int y = nowy + dy[i];
      bool diagonal = false;
      if (dx[i] && dy[i]) diagonal = true;
      if (! this->check(make_pair(x, y), make_pair(dx[i], dy[i]))) continue;
      dfs(make_pair(x, y), diagonal ? cost + 14 : cost + 10, routes, time + 1);
      this->vis[x][y] = false;
      process.push_back(Step(Step::convert_to_code(make_pair(nowx, nowy), this->size.second), time, cost));
    }
    routes.pop_back();
  }

  bool Solution::check(const pair<int, int>& coordinate, const pair<int, int>& direction) {
    int x = coordinate.first, y = coordinate.second;
    int height = this->size.first, width = this->size.second;

    bool diagonal = false;
    int dx = direction.first, dy = direction.second;
    if (dx != 0 && dy != 0) diagonal = true;
    
    
    if (x < 0 || y < 0 || x >= height || y >= width) return false;
    
    if (this->maze[x][y]) return false;
    
    if (diagonal && this->maze[x - dx][y] && this->maze[x][y - dy]) return false;
    
    if (this->vis[x][y]) return false;

    return true;

  }

  Solution solve(
    const pair<int, int>& start,
    const pair<int, int>& finish,
    const vector<vector<int> >& maze,
    const pair<int, int>& size)
  {
    Solution solution(start, finish, maze, size);

    vector<Step> temp;
    solution.dfs(start, 0, temp, 1);
    
    return solution;
  }
}