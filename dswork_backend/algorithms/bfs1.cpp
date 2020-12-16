#include<cstdlib>
#include<vector>
#include<utility>
#include<queue>
#include<stack>
#include<cmath>
#include"./classes/step.h"
using namespace std;
namespace bfs
{	
		class Node
	{   		
		    int x,y;
		public:			  
		    Node *pre;
		    int layer;
			Node(){}
			Node(pair<int,int> point):x(point.first),y(point.second){}			
			Node(int a,int b):x(a),y(b){}
			int getx(){ return x;}
			int gety(){ return y;}
			void setx(int a) {x=a;}
			void sety(int b) {y=b;}												  										  
	};
	class Node;
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
			vector<Step> process;
			int min_cost ;
		public:
			Solution() {}
			Solution(const pair<int, int>, const pair<int, int>, const vector<vector<int> >, const pair<int, int>); 
			int get_min_cost();
			vector<Step >& get_routes();
			vector<Step >& get_process(); 
			void bfs();
			bool check(const pair<int, int>&, const pair<int, int>&);
	};

	Solution::Solution(const pair<int, int> start, const pair<int, int> finish, const vector<vector<int> > maze, const pair<int, int> size)
		: start(start), finish(finish), maze(maze), size(size) 
		{
		min_cost = 0x7fffffff;
		vis.resize(size.first);
		for (vector<vector<bool> >::iterator i = vis.begin(); i != vis.end(); ++i) 
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
		
	vector<Step >& Solution::get_process()
		{
			return process;
		}	

	void Solution::bfs()  
		{			
			Node start1(start); 
			start1.pre=NULL;
			queue<Node> que;
			int depth;
			int temporary=size.first>size.second?size.first:size.second;
			depth=pow(3,temporary)+pow(temporary,3);
			Node *a=new Node[depth];
			int count=0;
			start1.layer=1;
			que.push(start1);
			Node final; 
			while(!que.empty()) 
			{ 
				while(!que.empty())
			{
				count++;
				a[count]=que.front();				
				Node cur = que.front();
				que.pop();
				if(make_pair(cur.getx(),cur.gety())==finish)
				{
					break;
				}
				for(int i=0;i<8;i++)
				{					 
					Node tmp;
					tmp.setx(cur.getx()+dx[i]);
					tmp.sety(cur.gety()+dy[i]);
					if(check(make_pair(cur.getx(),cur.gety()),make_pair(dx[i],dy[i])))
					{
						tmp.pre=&a[count];
						tmp.layer=a[count].layer+1;
						que.push(tmp);
					}
				}
				vis[cur.getx()][cur.gety()]=true;		
			}
				if(a[count].getx()==finish.first&&a[count].gety()==finish.second)
				{				
				Node c=a[count];
				int cost=0; 
				while(c.pre)
				{
					if(cost>min_cost) break;
					else if(c.getx()!=(*c.pre).getx()&&c.gety()!=(*c.pre).gety()) cost+=14;
					else cost+=10;
					c=*c.pre;
				}
				if(cost<min_cost) 
				{
					final=a[count];
					min_cost=cost;
				}
				}
			} 
			if(min_cost== 0x7fffffff) 
			{
				routes1.clear();
				
			}
			else
			{
			for(int i=1;i<=count;i++)
			{
				int cost=0;
				Node temp=a[i];
				while(temp.pre)
				{
					if((*a[i].pre).getx()!=a[i].getx()&&(*a[i].pre).gety()!=a[i].gety())
					cost+=14;
					else cost+=10;
					temp=*temp.pre;
				}
				Step step;
				step.set_coordinate(step.convert_to_code(make_pair(a[i].getx(),a[i].gety()),size.second));
				step.set_actual_cost(cost);
				step.set_estimate_cost(-1);
				step.set_time(a[i].layer);
				process.push_back(step);				
			}
						  
			routes1.push_back(start);
			routes.push_back(Step(Step::convert_to_code(start, size.second),1));
			stack<Node> sta;
			while(final.pre)
			{
				sta.push(final);
				final=*final.pre;
			}
			min_cost=0;
			int record=1;
			while(!sta.empty())
			{
				record++;
				int tx= sta.top().getx();
				int ty= sta.top().gety();							
				if(routes1.size()>=1)
				{
					if(routes1.back().first!=tx&&routes1.back().second!=ty)
					min_cost+=14;
					else min_cost+=10;				
				}
				routes1.push_back(make_pair(tx,ty));
				Step step;
				step.set_coordinate(step.convert_to_code(make_pair(tx,ty),size.second));
				step.set_actual_cost(min_cost);
				step.set_estimate_cost(-1);
				step.set_time(record);
				routes.push_back(step);
				sta.pop();
			}
			}
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
			if (make_pair(x,y)==finish) return false;
			
			return true;
		}
	Solution solve(
      const std::pair<int, int>& start,
      const std::pair<int, int>& finish,
      const std::vector<std::vector<int> >& maze,
      const std::pair<int, int>& size)
    {
      Solution solution(start, finish, maze, size);
			
      solution.bfs();
      
      return solution;
    }
}