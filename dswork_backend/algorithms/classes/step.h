#pragma once
#include <utility>

class Step {
private:
	int coordinate_code = -1; 
	int actual_cost = -1; 
	int estimate_cost = -1; 
	int time; 
public:
	Step() {}
	Step(const int&, const int&, const int&, const int&);
	static int convert_to_code(const std::pair<int, int>&, const int&);

	void set_coordinate(const int&);
	void set_actual_cost(const int&);
	void set_estimate_cost(const int&);
	void set_time(const int&);
	int get_coordinate_code();
	int get_coordinate_code() const;
	int get_time();
	std::pair<int, int> get_coordinate(const int&);
	int get_actual_cost();
	int get_estimate_cost();
	int get_total_cost();
	int get_total_cost() const;

};

Step::Step(const int& coordinate_code, const int& time = -1, const int& actual_cost = -1, const int& estimate_cost = -1)
	: coordinate_code(coordinate_code), time(time), actual_cost(actual_cost), estimate_cost(estimate_cost) {}


int Step::convert_to_code(const std::pair<int, int>& coordinate, const int& width) {
	return coordinate.first * width + coordinate.second + 1;
}

void Step::set_coordinate(const int& coordinate_code) {
	this->coordinate_code = coordinate_code;
}

void Step::set_actual_cost(const int& actual_cost) {
	this->actual_cost = actual_cost;
}

void Step::set_estimate_cost(const int& estimate_cost) {
	this->estimate_cost = estimate_cost;
}

void Step::set_time(const int& time) {
	this->time = time;
}

int Step::get_coordinate_code() {
	return this->coordinate_code;
}

int Step::get_coordinate_code() const {
	return this->coordinate_code;
}

std::pair<int, int> Step::get_coordinate(const int& width) {
	return std::make_pair((this->coordinate_code - 1) / width, (this->coordinate_code - 1) % width);
}

int Step::get_actual_cost() {
	return this->actual_cost;
}

int Step::get_estimate_cost() {
	return this->estimate_cost;
}

int Step::get_total_cost() {
	return this->actual_cost + this->estimate_cost;
}

int Step::get_total_cost() const {
	return this->actual_cost + this->estimate_cost;
}

int Step::get_time() {
	return this->time;
}