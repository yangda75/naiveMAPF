#pragma once
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

//////////////////////////////////////////////////////////////////////
// definition
//////////////////////////////////////////////////////////////////////

typedef int Cost;

struct GridPoint {
    int x{0};
    int y{0};
    GridPoint() : x{0}, y{0} {}
    GridPoint(int xx, int yy) : x{xx}, y{yy} {}
    bool operator==(const GridPoint &p) const { return p.x == x && p.y == y; }
};

struct Node {
    GridPoint point;
    Cost g;
    Cost f;
    GridPoint parent;
    int timeStamp;
    Node(GridPoint p, Cost gg, Cost ff, GridPoint pa)
        : point(p), g(gg), f(ff), parent(pa), timeStamp(0) {}
    Node(GridPoint p, Cost gg, Cost ff, GridPoint pa, int t)
        : point(p), g(gg), f(ff), parent(pa), timeStamp(t) {}
};
// used to sort Nodes, very slow, but easy to use
struct greater {
    bool operator()(const Node &a, const Node &b) { return a.f > b.f; }
};

struct Constraint {
    int agent;
    GridPoint point;
    int constraintTimeStamp;
    Constraint() {}
    Constraint(int a, GridPoint p, int t)
        : agent(a), point(p), constraintTimeStamp(t) {}
    Constraint(GridPoint p, int t) : point(p), constraintTimeStamp(t) {}
    bool operator==(const Constraint &cc) const {
        return cc.agent == agent && cc.point == point &&
               cc.constraintTimeStamp == constraintTimeStamp;
    }
};

class AStar {
   private:
    int dimX, dimY;
    std::vector<Node> openSet;
    std::vector<Node> closedSet;
    std::vector<GridPoint> obstacles;
    std::vector<Constraint> constraints;
    int timeStamp;
    Cost finalCost;

   public:
    AStar(int xx, int yy, std::vector<GridPoint> &o,
          std::vector<Constraint> &cc)
        : dimX(xx),
          dimY(yy),
          obstacles(o),
          constraints(cc),
          timeStamp(0),
          finalCost(0) {}
    std::vector<GridPoint> search(GridPoint &start, GridPoint &goal);
    Cost getCost(GridPoint p1, GridPoint p2);
    std::vector<GridPoint> getAdjacentGridPoints(GridPoint &p);
    Cost heuristic(GridPoint s, GridPoint g);
    Node getParent(GridPoint p);
    Cost getFinalCost();
};

///////////////////////////////////////////////////////////////////
// implementation
///////////////////////////////////////////////////////////////////
std::vector<GridPoint> AStar::search(GridPoint &start, GridPoint &goal) {
    std::cout << "searching with constraints: \n";
    for (Constraint c : constraints) {
        std::cout << "agent: " << c.agent << " point: " << c.point.x << ","
                  << c.point.y << " time: " << c.constraintTimeStamp << "\n";
    }
    std::vector<GridPoint> path;
    Cost h = heuristic(start, goal);
    Node startNode = Node(start, 0, h, start,0);
    openSet.push_back(startNode);
    // begin search
    // int cnt = 0;
    while (! openSet.empty()) {
        std::sort(openSet.begin(), openSet.end(), greater());
        Node currentNode = openSet.back();
        openSet.pop_back();  // delete the smallest element
        // push into closedSet
        closedSet.push_back(currentNode);
	timeStamp = currentNode.timeStamp;
        if (currentNode.point.x == goal.x && currentNode.point.y == goal.y) {
            // goal point found
            finalCost = currentNode.f;
            Node n = currentNode;
            while (n.point.x != start.x || n.point.y != start.y) {
                if (n.parent == n.point) break;
                path.push_back(n.point);
                n = getParent(n.point);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }
        std::vector<GridPoint> childrenGridPoints =
            getAdjacentGridPoints(currentNode.point);
        std::vector<GridPoint> closedGridPoints;
        closedGridPoints.resize(closedSet.size());
        std::transform(closedSet.begin(), closedSet.end(),
                       closedGridPoints.begin(),
                       [](Node n) { return n.point; });
        // std::vector<GridPoint> openGridPoints;
        // openGridPoints.resize(openSet.size());
        // std::transform(openSet.begin(), openSet.end(),
        // openGridPoints.begin(),
        //                [](Node n) { return n.point; });
        for (GridPoint childGridPoint : childrenGridPoints) {
            auto it = std::find(closedGridPoints.begin(),
                                closedGridPoints.end(), childGridPoint);
            // if point is in closed GridPoints
            if (it != closedGridPoints.end()) continue;
            Cost newg =
                currentNode.g + getCost(childGridPoint, currentNode.point);
            Cost newf = newg + heuristic(childGridPoint, goal);
            Node child = Node(childGridPoint, newg, newf, currentNode.point,
                              currentNode.timeStamp + 1);
            // std::cout << "making new node with point " << childGridPoint.x <<
            // ","
            //           << childGridPoint.y << std::endl;
            bool flag = true;

            for (Node n : openSet) {
                if (n.point == childGridPoint) {
                    flag = false;
                    if (child.f < n.f) {
                        n.g = child.g;
                        n.f = child.f;
			n.timeStamp = child.timeStamp;
                        // std::cout << "changing " << n.point.x << "," <<
                        // n.point.y
                        //           << " 's parent from " << n.parent.x << ","
                        //           << n.parent.y << " to " << child.parent.x
                        //           << ","
                        //           << child.parent.y << std::endl;
                        n.parent = child.parent;
                    }
                }
            }
            if (flag) {
                openSet.push_back(child);
            }
        }
    }
    return path;
}

std::vector<GridPoint> AStar::getAdjacentGridPoints(GridPoint &p) {
    std::vector<GridPoint> adjGridPoints;
    int xx[] = {-1, 0, 1};
    int yy[] = {-1, 0, 1};
    for (int dx : xx)
        for (int dy : yy)
            // TODO: implement WAIT operation where dx = dy =0
            // maybe adding 'timeStamp' field to Node works
            if (std::abs(dx + dy) == 1) {
                int x = dx + p.x;
                int y = dy + p.y;
                if (x >= 0 && x < dimX && y >= 0 && y < dimY) {
                    GridPoint newPoint(x, y);
                    if (std::find(obstacles.begin(), obstacles.end(),
                                  newPoint) == obstacles.end()) {
                        bool flag = true;
                        for (Constraint c : constraints)
                            if (c.point == newPoint &&
                                c.constraintTimeStamp == timeStamp + 1)
                            // not allowed to go to newPoint
                            {
                                std::cout << "constrained, not adding point "
                                          << newPoint.x << "," << newPoint.y
                                          << "\n";
                                flag = false;
                                break;
                            }
                        if (flag) adjGridPoints.push_back(newPoint);
                    }
                }
            }
    if (! constraints.empty()) {
        std::cout << "at timestamp " << timeStamp << "current point: " << p.x
                  << "," << p.y << ", adding points: \n";
        for (auto pp : adjGridPoints) {
            std::cout << pp.x << "," << pp.y << " ";
        }
        std::cout << "\n";
    }
    return adjGridPoints;
}

Cost AStar::getCost(GridPoint p1, GridPoint p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

Cost AStar::heuristic(GridPoint s, GridPoint g) {
    return std::abs(s.x - g.x) + std::abs(s.y - g.y);
}

Node AStar::getParent(GridPoint p) {
    Node newNode(GridPoint(-1, -1), -1, -1, GridPoint(-1, -1));
    for (Node n : closedSet) {
        if (n.point == p) {
            newNode = n;
            break;
        }
    }
    if (newNode.f == -1) {
        std::cout << "node not found";
        return newNode;
    }
    for (Node n : closedSet) {
        if (n.point == newNode.parent) {
            return n;
        }
    }
    std::cout << "node not found";
    return newNode;
}

Cost AStar::getFinalCost() { return finalCost; }
