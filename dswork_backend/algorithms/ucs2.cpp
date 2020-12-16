#include <vector>
#include<iostream>
#include<fstream>
#include<string>
#include<utility>
#include<algorithm>
#include<stack>
#include<queue>
#include"./classes/step.h"
using namespace std;
namespace ucs {
    class Solution
    {private:
        const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
        const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
        const int cost[8] ={10,14,10,14,10,14,10,14};
        pair<int, int>start;
        pair<int, int>finish;
        vector<vector<int> > maze;
        pair<int, int>size;
        vector<vector<bool> > vis;
        vector<vector<int> > totalcost;
        queue<pair<int, int> > openlist;
        vector<Step> routes;
    public:
        Solution(){}
        Solution(const pair<int, int>, const pair<int, int>, const vector<vector<int> >, const pair<int, int>);
        bool check(const pair<int, int>& , const pair<int, int>& );
        bool ucs();
        vector<Step> get_routes();
        void show_openlist();
    };

    void Solution::show_openlist()
    {

    }

    vector<Step> Solution:: get_routes()
    {return routes;}

    Solution::Solution(const pair<int, int> start, const pair<int, int> finish, const vector<vector<int> > maze, const pair<int, int> size)
        : start(start), finish(finish), maze(maze), size(size)
    {
        this->vis.resize(size.first);
        for (vector<vector<bool> >::iterator i = this->vis.begin(); i != this->vis.end(); ++i) {
            for (int j = 0; j < size.second; ++j) {
                (*i).push_back(false);
            }
        }
        this->totalcost.resize(size.first);
        for (vector<vector<int> >::iterator i = this->totalcost.begin(); i != this->totalcost.end(); ++i) {
            for (int j = 0; j < size.second; ++j) {
                (*i).push_back(9999);
            }
        }
    }

    bool Solution::check(const pair<int, int>& coordinate, const pair<int, int>& direction) {
        int height = this->size.first, width = this->size.second;
        int x = coordinate.first, y = coordinate.second;
        int dx = direction.first, dy = direction.second;

        if (dx < 0 || dy < 0 || dx >= height || dy >= width ) return false;
        
        if (this->maze[dx][dy]) return false;

        if ((dx-x)!=0 && (dy-y)!=0) 
        if(this->maze[dx][y]&&this->maze[x][dy])
            return false;

        if (this->vis[dx][dy]) return false;
            return true;
    }

    bool Solution::ucs()
    {int nowx=start.first,nowy=start.second;
    this->totalcost[nowx][nowy]=0;
    openlist.push(make_pair(nowx,nowy));
    while(openlist.empty()==false)
    {if(nowx==finish.first && nowy==finish.second)
        {
            Step temp(Step::convert_to_code(make_pair(nowx,nowy),size.second),this->totalcost[nowx][nowy],-1);
            routes.push_back(temp);
            openlist.pop();
            vis[nowx][nowy]=true;
            return true;
        } 
        for (int i=0;i<8;i++)
            {int Dx=nowx+dx[i],Dy=nowy+dy[i];
                if (check(make_pair(nowx,nowy),make_pair(Dx,Dy)))
                    {if(this->totalcost[Dx][Dy]==9999){ openlist.push(make_pair(Dx,Dy));       }
                        if(this->totalcost[Dx][Dy]>this->totalcost[nowx][nowy]+cost[i])
                            {int sum=this->totalcost[nowx][nowy]; this->totalcost[Dx][Dy]=sum+cost[i];}
                    }
            }
        Step temp(Step::convert_to_code(make_pair(nowx,nowy),size.second),this->totalcost[nowx][nowy],-1);
        routes.push_back(temp);
        openlist.pop();
        vis[nowx][nowy]=true;
        int min_cost=9999;
        for(int i=0;i<openlist.size();i++)
            {   Step temp1(Step::convert_to_code(make_pair(openlist.front().first,openlist.front().second),size.second),this->totalcost[openlist.front().first][openlist.front().second],-1);
                if(min_cost>this->totalcost[openlist.front().first][openlist.front().second])
                min_cost=this->totalcost[openlist.front().first][openlist.front().second];
                openlist.push(openlist.front());openlist.pop();
            }
        for(int i=0;i<openlist.size();i++)
            {if(min_cost==this->totalcost[openlist.front().first][openlist.front().second])
                {nowx=openlist.front().first;
                nowy=openlist.front().second;
                break;
            }
            else
                {openlist.push(openlist.front());openlist.pop();}
            }
    }
    return false;
    }

    Solution solve(
      const std::pair<int, int>& start,
      const std::pair<int, int>& finish,
      const std::vector<std::vector<int> >& maze,
      const std::pair<int, int>& size)
    {
      Solution solution(start, finish, maze, size);

      std::vector<Step> temp;
      solution.ucs();
      
      return solution;
    }

}