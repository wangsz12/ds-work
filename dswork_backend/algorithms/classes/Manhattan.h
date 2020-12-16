#pragma once
#include <utility>
#include <cmath>
using namespace std;

class Manhattan {
public:
	static int get_new_estimate_cost(const pair<int, int>&, const pair<int, int>&);
};

int Manhattan::get_new_estimate_cost(const pair<int, int>& now, const pair<int, int>& finish) {
	int dx = abs(now.first - finish.first);
	int dy = abs(now.second - finish.second);
	return dx + dy;
}