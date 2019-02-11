#include "CBS.hpp"

void test1() {
    //  0 1 2 3 4 5 6 7 8 9 x
    // 0 0,0,0,0,0,0,0,0,0,0;
    // 1 0,0,1,0,0,0,0,0,0,0;
    // 2 0,0,1,0,0,0,1,1,1,1;
    // 3 0,0,1,1,1,0,0,0,0,0;
    // 4 0,0,1,1,1,0,0,0,0,0;
    // 5 0,0,1,1,1,0,0,0,0,0;
    // 6 0,0,1,1,1,0,0,0,0,0;
    // 7 0,0,0,0,1,1,1,0,0,0;
    // 8 0,0,0,0,0,0,1,0,0,0;
    // 9 0,0,0,0,0,0,0,0,0,0;
    std::vector<GridPoint> obstacles;
    obstacles.push_back(GridPoint(2, 1));
    obstacles.push_back(GridPoint(2, 2));
    obstacles.push_back(GridPoint(2, 3));
    obstacles.push_back(GridPoint(2, 4));
    obstacles.push_back(GridPoint(2, 5));
    obstacles.push_back(GridPoint(2, 6));
    obstacles.push_back(GridPoint(3, 3));
    obstacles.push_back(GridPoint(3, 4));
    obstacles.push_back(GridPoint(3, 5));
    obstacles.push_back(GridPoint(3, 6));
    obstacles.push_back(GridPoint(4, 3));
    obstacles.push_back(GridPoint(4, 4));
    obstacles.push_back(GridPoint(4, 5));
    obstacles.push_back(GridPoint(4, 6));
    obstacles.push_back(GridPoint(4, 7));
    obstacles.push_back(GridPoint(5, 7));
    obstacles.push_back(GridPoint(6, 7));
    obstacles.push_back(GridPoint(6, 8));
    obstacles.push_back(GridPoint(6, 2));
    obstacles.push_back(GridPoint(7, 2));
    obstacles.push_back(GridPoint(8, 2));
    obstacles.push_back(GridPoint(9, 2));
    std::vector<GridPoint> starts;
    std::vector<GridPoint> goals;
    starts.push_back(GridPoint(3, 2));
    starts.push_back(GridPoint(9, 9));
    starts.push_back(GridPoint(9, 1));
    starts.push_back(GridPoint(9, 3));
    goals.push_back(GridPoint(9, 9));
    goals.push_back(GridPoint(1, 1));
    goals.push_back(GridPoint(1, 9));
    goals.push_back(GridPoint(2, 9));
    CBS cbsObj(4, 10, 10, obstacles, starts, goals);
    cbsObj.search();
    CTNode* solutionNode = cbsObj.getSolutionNode();
    for (auto path : solutionNode->solution) {
        for (GridPoint point : path) {
            std::cout << point.x << "," << point.y << "->";
        }
        std::cout <<"finish"<< std::endl;
    }
}

int main() {
    test1();
    return 0;
}
