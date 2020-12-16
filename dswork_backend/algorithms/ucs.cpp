/***************************************************************
 * @file       UCS
 * @brief      UCS
 * @author     Jin Chengyang
 * @version    1.0
 * @date       2020.11.22
 **************************************************************/

#include <utility>
#include <vector>
#include <queue>
#include "./classes/step.h"
using namespace std;

namespace ucs {
    class Solution
    {private:
        const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
        const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};
        const int cost[8] ={10,14,10,14,10,14,10,14};
        pair<int, int> start;
        pair<int, int> finish;
        vector<vector<int>> maze;
        pair<int, int> size;
        vector<vector<bool> > vis;
        vector<vector<int>> totalcost;
        vector<vector<int>> mintotalstep;
        queue<pair<int, int>> openlist;
        vector<Step> closedlist;
        vector<Step> route;

        vector<Step> process;

    public:
        Solution(){}
        Solution(const pair<int, int>, const pair<int, int>, const vector<vector<int> >, const pair<int, int>);
        bool check(const pair<int, int>& , const pair<int, int>& );
        bool checkbackroute(const pair<int, int>& , const pair<int, int>& );
        bool ucs();
        vector<Step> get_closedlist();
        vector<vector<int>> get_mintotalstep();
        void findroute();
        vector<Step> get_routes();
        vector<Step> get_process();
        
    };

    void Solution::findroute()
    {route.resize(mintotalstep[finish.first][finish.second]);
    int lastx=finish.first,lasty=finish.second;
    Step temp(Step::convert_to_code(make_pair(finish.first,finish.second),size.second),mintotalstep[finish.first][finish.second],this->totalcost[finish.first][finish.second],-1);
    route[mintotalstep[finish.first][finish.second]-1]=temp;
    Step temp0(Step::convert_to_code(make_pair(start.first,start.second),size.second),mintotalstep[start.first][start.second],this->totalcost[start.first][start.second],-1);
    route[0]=temp0;
    for(int i=mintotalstep[finish.first][finish.second]-2;i>0;i--)
        {int tempmincost=9999,j=lastx,k=lasty;
        for (int m=0;m<8;m++)
            {int Dx=j+dx[m],Dy=k+dy[m];
                if (checkbackroute(make_pair(j,k),make_pair(Dx,Dy))&&(mintotalstep[Dx][Dy]-1)==i&&totalcost[Dx][Dy]<tempmincost)
                    {lastx=Dx;lasty=Dy;tempmincost=totalcost[Dx][Dy];}
            }
        Step temp1(Step::convert_to_code(make_pair(lastx,lasty),size.second),mintotalstep[lastx][lasty],this->totalcost[lastx][lasty],-1);
        route[i]=temp1;
        }
    }

    vector<Step> Solution:: get_process()
    {return process;}

    vector<Step> Solution:: get_routes()
    {return route;}

    vector<vector<int>> Solution::get_mintotalstep()
    {return mintotalstep;}

    vector<Step> Solution:: get_closedlist()
    {return closedlist;}

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
        this->mintotalstep.resize(size.first);
        for (vector<vector<int> >::iterator i = this->totalcost.begin(); i != this->totalcost.end(); ++i) {
            for (int j = 0; j < size.second; ++j) {
                (*i).push_back(9999);
            }
        }
        for (vector<vector<int> >::iterator i = this->mintotalstep.begin(); i != this->mintotalstep.end(); ++i) {
            for (int j = 0; j < size.second; ++j) {
                (*i).push_back(999);
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

    bool Solution::checkbackroute(const pair<int, int>& coordinate, const pair<int, int>& direction) {
        int height = this->size.first, width = this->size.second;
        int x = coordinate.first, y = coordinate.second;
        int dx = direction.first, dy = direction.second;

        if (dx < 0 || dy < 0 || dx >= height || dy >= width ) return false;
        
        if (this->maze[dx][dy]) return false;

        if ((dx-x)!=0 && (dy-y)!=0) 
        if(this->maze[dx][y]&&this->maze[x][dy])
            return false;

        if (!this->vis[dx][dy]) return false;
            return true;
    }

    bool Solution::ucs()
    {int nowx=start.first,nowy=start.second;
    this->totalcost[nowx][nowy]=0;
    this->mintotalstep[nowx][nowy]=1;
    openlist.push(make_pair(nowx,nowy));
    Step tempprocess(Step::convert_to_code(make_pair(nowx,nowy),size.second),1,this->totalcost[nowx][nowy],-1);
        process.push_back(tempprocess);
    while(openlist.empty()==false)
    {if(nowx==finish.first && nowy==finish.second)
        {   Step temp(Step::convert_to_code(make_pair(nowx,nowy),size.second),mintotalstep[nowx][nowy],this->totalcost[nowx][nowy],-1);
            closedlist.push_back(temp);
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
                        if(this->mintotalstep[Dx][Dy]>this->mintotalstep[nowx][nowy]+1)
                            {int sum=this->mintotalstep[nowx][nowy]; this->mintotalstep[Dx][Dy]=sum+1;}
                    }
            }
        Step temp(Step::convert_to_code(make_pair(nowx,nowy),size.second),mintotalstep[nowx][nowy],this->totalcost[nowx][nowy],-1);
        closedlist.push_back(temp);
        openlist.pop();
        vis[nowx][nowy]=true;
        int min_cost=9999;
        for(int i=0;i<openlist.size();i++)
            {   Step temp1(Step::convert_to_code(make_pair(openlist.front().first,openlist.front().second),size.second),mintotalstep[openlist.front().first][openlist.front().second],this->totalcost[openlist.front().first][openlist.front().second],-1);
                if(min_cost>this->totalcost[openlist.front().first][openlist.front().second])
                min_cost=this->totalcost[openlist.front().first][openlist.front().second];
                process.push_back(temp1);
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
      if (solution.ucs()) {
        solution.findroute();
      }
      
      return solution;
    }

}