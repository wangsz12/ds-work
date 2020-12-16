/***************************************************************
 * @file       DFS
 * @brief      DFS
 * @author     Zhang Yinan
 * @version    1.0
 * @date       2020.11.22
 **************************************************************/

#include <vector>
#include <utility>
#include <cmath>
#include "./classes/step.h"
using namespace std;

namespace yinan 
{
	class Solution 
	{
		private:
			const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
			const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
			vector<vector<int> > maze;
			pair<int, int> size;
			pair<int, int> start;
			pair<int, int> finish;
			vector<vector<bool> > vis;			
			vector<Step > routes;
			vector<pair<int,int> > routes1;
			vector<vector<bool> > step_complete;
			int min_cost ;
		public:
			Solution() {}
			Solution(const pair<int, int>, const pair<int, int>, const vector<vector<int> >, const pair<int, int>);
			int get_min_cost();
			vector<Step >& get_routes();
			void dfs();
			bool check(const pair<int, int>&, const pair<int, int>&);
			pair<int, int> back(); 
	};

	Solution::Solution(const pair<int, int> start, const pair<int, int> finish, const vector<vector<int> > maze, const pair<int, int> size)
		: start(start), finish(finish), maze(maze), size(size) 
		{
		min_cost=0x7fffffff;
		vis.resize(size.first);
		for (vector<vector<bool> >::iterator i = vis.begin(); i != vis.end(); ++i) 
		{
			for (int j = 0; j < size.second; ++j)	(*i).push_back(false);			
		}
		step_complete.resize(size.first);
		for (vector<vector<bool> >::iterator i = step_complete.begin(); i != step_complete.end(); ++i) 
		{
			for (int j = 0; j < size.second; ++j)	(*i).push_back(false);			
		}
		}
 
	int Solution::get_min_cost() 
		{
			return min_cost;
		}

	vector<Step >& Solution::get_routes() 
		{
			return routes;
		}

	void Solution::dfs()  
		{
			int x=start.first;
			int y=start.second;
			int cost=0;
			Step step;
			step.set_coordinate(step.convert_to_code(start,size.second));
			step.set_actual_cost(0);
			step.set_estimate_cost(-1);
			routes.push_back(step);
			routes1.push_back(start);
			while(x!=finish.first||y!=finish.second)
			{
				int x1=x,y1=y;
				int i;
				if(cost>min_cost)	break;		
				for( i=0;i<8;i++)
				{
					if(check(make_pair(x,y),make_pair(dx[i],dy[i])))	vis[x][y]=true;
					else continue;
					if(x+dx[i]!=finish.first||y+dy[i]!=finish.second)
					{
						for(int j=0;j<8;j++)						
						if(check(make_pair(x+dx[i],y+dy[i]),make_pair(dx[j],dy[j])))
							{ 
								step_complete[x][y]=true;
								break; 
							}
					}
					else step_complete[x][y]=true;															
					if(step_complete[x][y])
					{ 
						x+=dx[i],y+=dy[i];
						if(dx[i]&&dy[i]) cost+=14;
						else cost+=10;
						break; 
					} 
					else continue;
												
				}				
				if(x==x1&&y==y1)
				{
					step_complete[x][y]=true;
					x=back().first; 
					y=back().second;
					if(x==start.first&&y==start.second &&i==8)	
					{
						
						break;	
					}
				} 
				else 
				{
					step.set_coordinate(step.convert_to_code(make_pair(x,y),size.second));
					step.set_actual_cost(cost);
					step.set_estimate_cost(-1);
					routes.push_back(step);
					routes1.push_back(make_pair(x,y));
				}						 
			} 
			
			
			if(x==start.first&&y==start.second)
			{
				routes1.clear();
				routes.clear();
				min_cost=0; 
			}			
			else if(cost<min_cost) min_cost=cost;
								
		}

	bool Solution::check(const pair<int, int>& coordinate, const pair<int, int>& direction) 
		{
			int x = coordinate.first, y = coordinate.second;
			int height = size.first, width = size.second;
			int dx = direction.first, dy = direction.second;
			bool diagonal = false;
			if (dx != 0 && dy != 0)  diagonal = true;
			if (x < 0 || y < 0 || x >= height || y >= width) return false;
			if (x+dx < 0 || y +dy< 0 ||x+dx >= height|| y +dy>= width) return false;
			if (maze[x+dx][y+dy]) return false;
			if (diagonal && maze[x + dx][y] &&maze[x][y + dy]) return false;
			if (vis[x+dx][y+dy]) return false;
			
			if(step_complete[x+dx][y+dy]) return false;
			return true;
		}
	pair<int, int> Solution::back()
		{
			if(routes1.size()!=1)
			{
				routes1.pop_back();
				routes.pop_back();
			}						
			return routes1.back();		
		}	

    Solution solve(
      const std::pair<int, int>& start,
      const std::pair<int, int>& finish,
      const std::vector<std::vector<int> >& maze,
      const std::pair<int, int>& size)
    {
      Solution solution(start, finish, maze, size);
			
      solution.dfs();
      
      return solution;
    }
}