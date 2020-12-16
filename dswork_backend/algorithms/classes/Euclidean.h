#pragma once
#include <utility>
#include <cmath>
using namespace std;

class Euclidean {
public:
	static int get_new_estimate_cost(const pair<int, int>&, const pair<int, int>&);
};

int Euclidean::get_new_estimate_cost(const pair<int, int>& now, const pair<int, int>& finish) {
	int dx = abs(now.first - finish.first);
	int dy = abs(now.second - finish.second);
	return static_cast<int>(floor(sqrt(dx * dx + dy * dy)));
}