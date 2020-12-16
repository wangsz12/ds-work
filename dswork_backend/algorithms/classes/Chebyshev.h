#pragma once
#include <utility>
#include <cmath>
using namespace std;

class Chebyshev {
public:
	static int get_new_estimate_cost(const pair<int, int>&, const pair<int, int>&);
};

int Chebyshev::get_new_estimate_cost(const pair<int, int>& now, const pair<int, int>& finish) {
	int dx = abs(now.first - finish.first);
	int dy = abs(now.second - finish.second);
	int max = dx >= dy ? dx : dy;
	int min = dx < dy ? dx : dy;
	return 10 * max + 4 * min;
}