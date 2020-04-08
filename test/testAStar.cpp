#include "../src/AStar.hpp"
#include <cstdlib>  // rand
#include <iomanip>  // setw
// int test1() {
//   int dimX = 1000;
//   int dimY = 1000;
//   GridPoint start(1, 1);
//   GridPoint goal(999, 999);
//   std::vector<GridPoint> obstacles;
//   for (int i = 0; i < 10; i++) {
//     obstacles.push_back(GridPoint(rand() % dimX, rand() % dimY));
//   }
//   AStar astar(dimX, dimY, obstacles);
//   std::vector<GridPoint> path = astar.search(start, goal);
//   // for (auto p : path) {
//   //   std::cout << "x: " << std::setw(8) << p.x << ", y:" << std::setw(8) <<
//   //   p.y
//   //             << "\n";
//   // }
//   return 0;
// }

void test2() {
  // params
  int dimX = 3;
  int dimY = 3;
  GridPoint start(1, 1);
  GridPoint goal(2, 2);
  Constraint c1(GridPoint(1, 2), 2);
  Constraint c2(GridPoint(2, 1), 2);
  std::vector<Constraint> constraints = {c1, c2};
  std::vector<GridPoint> obstacles    = {GridPoint(1, 0)};
  // end of params

  AStar astar(dimX, dimY, obstacles, constraints);
  std::vector<GridPoint> path = astar.search(start, goal);
  for (auto p : path) {
    std::cout << p.x << "," << p.y << " -> ";
  }
  std::cout << "finish";
}

int main() {
  test2();
  return 0;
}
