#include<iostream>
#include<utility>
#include<vector>
#include "./classes/step.h"

using namespace std;

namespace greedy {
	class Solution
	{
	private:
		pair<int,int> start;
		pair<int,int> finish;
		pair<int,int> size;
		pair<int,int> former_location;
		pair<int,int> current_location;
		vector<int> cost;
		vector<int> coordinate_code;
		vector<int> process_step;
		vector<int> process_coordinate_code;
		vector< vector<int> > maze;
		vector< vector<int> > current_maze;
		vector< pair<int,int> > route;
		vector< pair<int,int> > process;
		vector<Step> routes;
		vector<Step> processes;
		int steps;
		int process_steps;
	public:
		Solution(const vector< vector<int> >,const pair<int,int>,const pair<int,int>,const pair<int,int>);
		void go_next_step();
		void find_optimum();
		void calculate_cost();
		void convert_coordinate();
		void convert_process_coordinate();
		void convert_solution();
		void judge_solution();
		int judge_next_step(int,int);
		int judge_former_next_step(int,int);
		int calculate_distance(int,int);
		vector<Step>& get_routes();
		vector<Step>& get_process();
	};

	Solution::Solution(const vector< vector<int> > client_maze,const pair<int,int> client_start,const pair<int,int> client_finish,const pair<int,int> client_size)
	{
		for(int i=0;i<client_size.first;i++)
		{
			vector<int> row;
			for(int j=0;j<client_size.second;j++)
				row.push_back(client_maze[i][j]);
			maze.push_back(row);
			current_maze.push_back(row);
		}
		this->former_location=client_start;
		this->start=client_start;
		this->finish=client_finish;
		this->size=client_size;
		this->current_location=client_start;
		this->process_step.push_back(1);
		this->steps=1;
		this->process_steps=1;
		this->cost.push_back(0);
		this->process.push_back(client_start);
		this->route.push_back(client_start);
	}

	void Solution::go_next_step()
	{
		int judge,judge_process;
		do{
			judge=0;
			judge_process=1;
			this->current_maze[this->current_location.first][this->current_location.second]=1;
			this->find_optimum();
			this->process.push_back(this->former_location);
			this->process_steps++;
			for(int j=0;j<this->process_steps-1;j++)
				if(this->process[this->process_steps-1].first==this->process[j].first&&this->process[this->process_steps-1].second==this->process[j].second)
				{
					this->process_step.push_back(this->process_step[j]);
					judge_process=0;
					break;
				}
			if(judge_process)
				this->process_step.push_back(1+this->process_step[this->process_steps-2]);
			if(this->current_location.first==this->finish.first&&this->current_location.second==this->finish.second)
			{
				this->process.push_back(this->current_location);
				this->process_step.push_back(1+this->process_step[this->process_steps-1]);
				this->process_steps++;
				this->route.push_back(this->current_location);
				steps++;
				this->calculate_cost();
				judge=0;
			}
			else
			{
				int x[8] = {-1,-1,-1,0,0,1,1,1};
				int y[8] = {-1,0,1,-1,1,-1,0,1};
				for(int n=0;n<8;n++)
				{
					if(this->judge_former_next_step(x[n],y[n])==1)
					{
						judge=1;
						this->route.push_back(this->current_location);
								steps++;
						this->calculate_cost();
						break;
					}
				}
				if(judge==0&&steps>=2)
				{
					this->former_location.first=this->current_location.first;
					this->former_location.second=this->current_location.second;
					this->current_maze[this->current_location.first][this->current_location.second]=1;
					this->current_location.first=this->route[steps-2].first;
					this->current_location.second=this->route[steps-2].second;
					judge=1;
					this->cost.pop_back();
					this->route.pop_back();
					steps--;
				}
				if(judge==0&&steps==1)
				{
					this->former_location.first=this->current_location.first;
					this->former_location.second=this->current_location.second;
					this->route.clear();
					this->cost.clear();
					steps=0;
				}
			}
			if(this->former_location.first==this->current_location.first&&this->former_location.second==this->current_location.second)
				judge=0;
		}while(judge==1);
		this->convert_coordinate();
		this->convert_process_coordinate();
		this->convert_solution();
	}

	void Solution::find_optimum()
	{
		int x[8] = {-1,-1,-1,0,0,1,1,1};
		int y[8] = {-1,0,1,-1,1,-1,0,1};
		int n,change_x=0,change_y=0,judge=0;
		for(n=0;n<8;n++)
		{
			if(this->judge_next_step(x[n],y[n])==1)
			{
				judge=1;
				break;
			}
			else continue;
		}
		this->former_location.first=this->current_location.first;
		this->former_location.second=this->current_location.second;
		if(judge==1)
		{
			int min_distance=2020;
			for(n=0;n<8;n++)
			{
				if(this->judge_next_step(x[n],y[n])==1)
				{
					int distance;
					distance=this->calculate_distance(x[n],y[n]);
					if(distance<min_distance)
					{
						min_distance=distance;
						change_x=x[n];
						change_y=y[n];
					}
					else continue;
				}
				else continue;
			}
			this->current_location.first+=change_x;
			this->current_location.second+=change_y;
		}
	}

	void Solution::calculate_cost()
	{
		int dx,dy,distance;
		dx=this->route[steps-1].first-this->route[steps-2].first;
		dy=this->route[steps-1].second-this->route[steps-2].second;
		distance=dx*dx+dy*dy;
		if(distance==2)
		{
			int its_cost;
			its_cost=this->cost[steps-2]+14;
			this->cost.push_back(its_cost);
		}
		else
		{
			int its_cost;
			its_cost=this->cost[steps-2]+10;
			this->cost.push_back(its_cost);
		}
	}

	void Solution::convert_coordinate()
	{
		for(int i=0;i<this->steps;i++)
		{
			int temp;
			temp=this->route[i].first*this->size.second+this->route[i].second+1;
			this->coordinate_code.push_back(temp);
		}
	}

	void Solution::convert_process_coordinate()
	{
		for(int i=0;i<this->process_steps;i++)
		{
			int temp;
			temp=this->process[i].first*this->size.second+this->process[i].second+1;
			this->process_coordinate_code.push_back(temp);
		}
	}

	void Solution::convert_solution()
	{
		for(int i=0;i<this->steps;i++)
			this->routes.push_back(Step(this->coordinate_code[i],-1,this->cost[i]));
		for(int i=1;i<this->process_steps;i++)
			this->processes.push_back(Step(this->process_coordinate_code[i],this->process_step[i]));
	}

	int Solution::judge_next_step(int x,int y)
	{
		this->current_maze[this->former_location.first][this->former_location.second]=1;
		if(this->current_location.first+x>=0&&this->current_location.second+y>=0&&this->current_location.first+x<this->size.first&&this->current_location.second+y<this->size.second)
		{
			if(x!=0&&y!=0&&this->current_maze[this->current_location.first+x][this->current_location.second+y]==0)
			{
				pair<int,int> dx(this->current_location.first+x,this->current_location.second);
					pair<int,int> dy(this->current_location.first,this->current_location.second+y);
					if(this->current_maze[dx.first][dx.second]==0||this->current_maze[dy.first][dy.second]==0)
				return 1;
				else return 0;
			}
			else if(this->current_maze[this->current_location.first+x][this->current_location.second+y]==0)
				return 1;
			else return 0;
		}
		else return 0;
	}

	int Solution::judge_former_next_step(int x,int y)
	{
		this->current_maze[this->former_location.first][this->former_location.second]=1;
		if(this->former_location.first+x>=0&&this->former_location.second+y>=0&&this->former_location.first+x<this->size.first&&this->former_location.second+y<this->size.second)
		{
			if(x!=0&&y!=0&&this->current_maze[this->former_location.first+x][this->former_location.second+y]==0)
			{
				pair<int,int> dx(this->former_location.first+x,this->former_location.second);
					pair<int,int> dy(this->former_location.first,this->former_location.second+y);
					if(this->current_maze[dx.first][dx.second]==0||this->current_maze[dy.first][dy.second]==0)
				return 1;
				else return 0;
			}
			else if(this->current_maze[this->former_location.first+x][this->former_location.second+y]==0)
				return 1;
			else return 0;
		}
		else return 0;
	}

	int Solution::calculate_distance(int x,int y)
	{
		int dx,dy;
		dx=x+this->current_location.first-this->finish.first;
		dy=y+this->current_location.second-this->finish.second;

		return dx*dx+dy*dy;
	}

	vector<Step>& Solution::get_routes()
	{
		return this->routes;
	}

	vector<Step>& Solution::get_process()
	{
		return this->processes;
	}


	Solution solve(
      const std::pair<int, int>& start,
      const std::pair<int, int>& finish,
      const std::vector<std::vector<int> >& maze,
      const std::pair<int, int>& size)
    {
      Solution solution(maze, start, finish, size);

			solution.go_next_step();
      
      return solution;
    }
}