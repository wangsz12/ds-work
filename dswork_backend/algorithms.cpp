#include <napi.h>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cstdlib>
#include "./algorithms/classes/step.h"
using namespace Napi;
using namespace std;

#include "algorithms/test_dfs.cpp"
#include "algorithms/ucs.cpp"
#include "algorithms/dfs.cpp"
#include "algorithms/bfs1-1.cpp"
#include "algorithms/bfs.cpp"
#include "algorithms/greedy2.cpp"
#include "algorithms/astar_euc.cpp"
#include "algorithms/astar_man.cpp"
#include "algorithms/astar_che.cpp"

#define log(x) (cout<<(#x)<<": "<<(x)<<endl)

void get_params(
  const CallbackInfo& info,
  int& algorithm,
  vector<vector<int> >& maze,
  pair<int, int>& size,
  pair<int, int>& start,
  pair<int, int>& finish)
{
  Env env = info.Env();

  algorithm = info[0].As<Number>().Int32Value();
  cout << "[C++] algorithm: " << algorithm << endl;

  Object js_size = info[2].As<Object>();
  int height = static_cast<Value>(js_size["height"]).As<Number>().Int32Value();
  int width = static_cast<Value>(js_size["width"]).As<Number>().Int32Value();
  size = make_pair(height, width);
  cout << "[C++] height: " << height << ", width: " << width << endl;

  Array js_maze = info[1].As<Array>();
  maze.resize(height);
  int r = 0, len;
  for (vector<vector<int> >::iterator i = maze.begin(); i != maze.end(); ++i) {
    Array row = static_cast<Value>(js_maze[r++]).As<Array>();
    len = row.Length();
    for (int j = 0; j < len; j++) {
      (*i).push_back(static_cast<Value>(row[j]).As<Number>().Int32Value());
    }
  }

  Object js_start = info[3].As<Object>();
  start = make_pair(static_cast<Value>(js_start["x"]).As<Number>().Int32Value(), static_cast<Value>(js_start["y"]).As<Number>().Int32Value());
  cout << "[C++] start.x: " << start.first << ", start.y: " << start.second << endl;

  Object js_finish = info[4].As<Object>();
  finish = make_pair(static_cast<Value>(js_finish["x"]).As<Number>().Int32Value(), static_cast<Value>(js_finish["y"]).As<Number>().Int32Value());
  cout << "[C++] finish.x: " << finish.first << ", finish.y: " << finish.second << endl;
}

Object solve_maze(const CallbackInfo& info) {
  const Env env = info.Env();

  int algorithm;
  vector<vector<int> > maze;
  pair<int, int> size;
  pair<int, int> start;
  pair<int, int> finish;

  get_params(info, algorithm, maze, size, start, finish);

  vector<Step> routes;
  vector<Step> process;
  int min_cost = -1;

  switch(algorithm) {
    case 1: {
      yinan::Solution result = yinan::solve(start, finish, maze, size);
      routes = result.get_routes();
      min_cost = result.get_min_cost();
      break;
    }
    case 2: {
      bfs::Solution result = bfs::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 3: {
      greedy::Solution result = greedy::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 4: {
      ucs::Solution result = ucs::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 5: {
      astareuc::Solution result = astareuc::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 6: {
      astarman::Solution result = astarman::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 7: {
      astarche::Solution result = astarche::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 8: {
      Dfs::Solution result = Dfs::solve(start, finish, maze, size);
      routes = result.get_routes();
      process = result.get_process();
      break;
    }
    case 9: {
      oldbfs::Solution result = oldbfs::solve(start, finish, maze, size);
      routes = result.get_routes();
      min_cost = result.get_min_cost();
      break;
    }
    default: {
      TypeError::New(env, "algorithm not supported").ThrowAsJavaScriptException();
      return Object::New(env);
    }
  }

  if (routes.empty()) {
    routes.push_back(Step(-1, 0));
  }

  if (process.empty()) {
    process.push_back(Step(-1, 0));
  }

  cout << "[C++] routes size: " << routes.size() << endl;
  cout << "[C++] process size: " << process.size() << endl;

  Object response = Object::New(env);
  Object step, proc;
  Array steps = Array::New(env, routes.size());
  Array procs = Array::New(env, process.size());
  int cnt = 0;
  for (vector<Step>::iterator i = routes.begin(); i != routes.end(); ++i) {
    step = Object::New(env);
    step["code"] = Number::New(env, i->get_coordinate_code());
    step["actualCost"] = Number::New(env, i->get_actual_cost());
    steps.DefineProperties({ PropertyDescriptor::Value(to_string(cnt).c_str(), step, napi_writable) });
    cnt++;
	}

  cnt = 0;
  for (vector<Step>::iterator i = process.begin(); i != process.end(); ++i) {
    proc = Object::New(env);
    proc["code"] = Number::New(env, i->get_coordinate_code());
    proc["actualCost"] = Number::New(env, i->get_actual_cost());
    proc["time"] = Number::New(env, i->get_time());
    procs.DefineProperties({ PropertyDescriptor::Value(to_string(cnt).c_str(), proc, napi_writable) });
    cnt++;
	}

  response["cost"] = Number::New(env, min_cost);
  response["steps"] = steps;
  response["process"] = procs;

  return response;
}

Object Init(Env env, Object exports) {
  exports.Set("solveMazeByAlgo", Function::New(env, solve_maze));
  return exports;
}

NODE_API_MODULE(addon, Init)