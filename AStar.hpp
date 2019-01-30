#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

//////////////////////////////////////////////////////////////////////
// definition
//////////////////////////////////////////////////////////////////////

typedef int Cost;

struct GridPoint {
  int x;
  int y;
  GridPoint() {}  // without this line, there will be "candidate constructor not
                  // viable error"
  GridPoint(int xx, int yy) : x(xx), y(yy) {}
  bool operator==(const GridPoint& p) const { return p.x == x and p.y == y; }
};

struct Node {
  GridPoint point;
  Cost g;
  Cost f;
  GridPoint parent;
  Node(GridPoint p, Cost gg, Cost ff, GridPoint pa)
    : point(p), g(gg), f(ff), parent(pa) {}
};
// used to sort Nodes, very slow, but easy to use
struct greater {
  bool operator()(const Node& a, const Node& b) { return a.f > b.f; }
};

struct Constraint {
  GridPoint point;
  int constraintTimeStamp;
  Constraint() {}
  Constraint(GridPoint p, int t) : point(p), constraintTimeStamp(t) {}
};

class AStar {
 private:
  int dimX, dimY;
  std::vector<Node> openSet;
  std::vector<Node> closedSet;
  std::vector<GridPoint> obstacles;
  std::vector<Constraint> constraints;
  int timeStamp;

 public:
  AStar(int xx, int yy, std::vector<GridPoint> o, std::vector<Constraint> cc)
    : dimX(xx), dimY(yy), obstacles(o), constraints(cc), timeStamp(0) {}
  std::vector<GridPoint> search(GridPoint start, GridPoint goal);
  Cost getCost(GridPoint p1, GridPoint p2);
  std::vector<GridPoint> getAdjacentGridPoints(GridPoint p);
  Cost heuristic(GridPoint s, GridPoint g);
  Node getParent(GridPoint p);
};

///////////////////////////////////////////////////////////////////
// implementation
///////////////////////////////////////////////////////////////////
std::vector<GridPoint> AStar::search(GridPoint start, GridPoint goal) {
  std::vector<GridPoint> path;
  Cost h         = heuristic(start, goal);
  Node startNode = Node(start, 0, h, start);
  openSet.push_back(startNode);
  // begin search
  // int cnt = 0;
  while (not openSet.empty()) {
    timeStamp++;
    std::sort(openSet.begin(), openSet.end(), greater());
    Node currentNode = openSet.back();
    openSet.pop_back();  // delete the smallest element
    // push into closedSet
    closedSet.push_back(currentNode);
    if (currentNode.point.x == goal.x and currentNode.point.y == goal.y) {
      // goal point found
      Node n = currentNode;
      while (n.point.x != start.x or n.point.y != start.y) {
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
    std::transform(closedSet.begin(), closedSet.end(), closedGridPoints.begin(),
                   [](Node n) { return n.point; });
    // std::vector<GridPoint> openGridPoints;
    // openGridPoints.resize(openSet.size());
    // std::transform(openSet.begin(), openSet.end(), openGridPoints.begin(),
    //                [](Node n) { return n.point; });
    for (GridPoint childGridPoint : childrenGridPoints) {
      auto it = std::find(closedGridPoints.begin(), closedGridPoints.end(),
                          childGridPoint);
      // if point is in closed GridPoints
      if (it != closedGridPoints.end()) continue;
      Cost newg  = currentNode.g + getCost(childGridPoint, currentNode.point);
      Cost newf  = newg + heuristic(childGridPoint, goal);
      Node child = Node(childGridPoint, newg, newf, currentNode.point);
      // std::cout << "making new node with point " << childGridPoint.x << ","
      //           << childGridPoint.y << std::endl;
      bool flag = true;

      for (Node n : openSet) {
        if (n.point == childGridPoint) {
          flag = false;
          if (child.f < n.f) {
            n.g = child.g;
            n.f = child.f;
            // std::cout << "changing " << n.point.x << "," << n.point.y
            //           << " 's parent from " << n.parent.x << ","
            //           << n.parent.y << " to " << child.parent.x << ","
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

std::vector<GridPoint> AStar::getAdjacentGridPoints(GridPoint p) {
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
        if (x >= 0 and x < dimX and y >= 0 and y < dimY) {
          GridPoint newPoint(x, y);
          if (std::find(obstacles.begin(), obstacles.end(), newPoint) ==
              obstacles.end()) {
            bool flag = true;
            for (Constraint c : constraints)
              if (c.point == newPoint and
                  c.constraintTimeStamp == timeStamp + 1)
              // not allowed to go to newPoint
              {
                flag = false;
                break;
              }
            if (flag) adjGridPoints.push_back(newPoint);
          }
        }
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
