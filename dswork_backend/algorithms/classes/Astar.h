#pragma once
#include <vector>
#include <list>
using namespace std;

const int kCost1 = 10; 
const int kCost2 = 14; 

class Solution {
private:
	std::vector<std::vector<int> > maze;
	std::pair<int, int> size;
	std::pair<int, int> start;
	std::pair<int, int> finish;
	vector<pair<int, int>> routes;
public:
	Solution() {}
	Solution(const std::pair<int, int>, const std::pair<int, int>, const std::vector<std::vector<int> >, const std::pair<int, int>);
	
	int get_min_cost();
	void get_routes(int x1, int y1);
	void clear_routes();
	void print_routes();
};

struct Point
{
	int x, y; 
	int F, G, H; 
	Point* parent; 
	Point(int _x, int _y) :x(_x), y(_y), F(0), G(0), H(0), parent(NULL)  
	{
	}
};

class Astar
{
public:
	void InitAstar(std::vector<std::vector<int>>& _maze);
	std::list<Point*> GetPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner);

private:
	Point* findPath(Point& startPoint, Point& endPoint, bool isIgnoreCorner);
	std::vector<Point*> getSurroundPoints(const Point* point, bool isIgnoreCorner) const;
	bool isCanreach(const Point* point, const Point* target, bool isIgnoreCorner) const; 
	Point* isInList(const std::list<Point*>& list, const Point* point) const; 
	Point* getLeastFpoint(); 
	
	int calcG(Point* temp_start, Point* point);
	int calcH(Point* point, Point* end);
	int calcF(Point* point);
private:
	std::vector<std::vector<int>> maze;
	std::list<Point*> openList;  
	std::list<Point*> closeList; 
};