#pragma once
#include "AStar.hpp"

//////////////////////////////////////////////////////////////////////
// definition
//////////////////////////////////////////////////////////////////////

struct Conflict {
    int agent1, agent2;
    GridPoint point;
    int timeStamp;
    Conflict(int a1, int a2, GridPoint p, int t)
        : agent1(a1), agent2(a2), point(p), timeStamp(t) {}
};

// Constraint Tree Node
struct CTNode {
    Cost cost;
    std::vector<Cost> costs;
    std::vector<Constraint> constraints;
    std::vector<std::vector<GridPoint>> solution;
    Conflict getFirstConflict();
    CTNode *leftChild, *rightChild, *parent;
    CTNode() {}
    CTNode(Cost c, std::vector<Constraint> &con,
           std::vector<std::vector<GridPoint>> &s, std::vector<Cost> cc)
        : cost(c),
          constraints(con),
          solution(s),
          costs(cc),
          leftChild(nullptr),
          rightChild(nullptr),
          parent(nullptr) {}
};

class CBS {
   private:
    int numberOfAgents;
    int dimX, dimY;
    std::vector<GridPoint> obstacles;
    std::vector<GridPoint> starts;
    std::vector<GridPoint> goals;
    CTNode *root;
    CTNode *solutionNode;
    int cnt;

   public:
    CBS(int n, int xx, int yy, std::vector<GridPoint> &o,
        std::vector<GridPoint> &s, std::vector<GridPoint> &g)
        : numberOfAgents(n),
          dimX(xx),
          dimY(yy),
          obstacles(o),
          starts(s),
          goals(g),
          root(nullptr),
          cnt(0) {}
    void splitOnConflict(Conflict con, CTNode *node);
    void search();
    void lowLevelSearch(int agent, GridPoint start, GridPoint goal);
    void getConstraints4Agent(int agent, std::vector<Constraint> &cc);
    CTNode *getSolutionNode() { return solutionNode; }
    void onNewNode();
};

//////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////
void CBS::onNewNode() {
    cnt++;
    if (cnt % 100 == 0) std::cout << "already checked " << cnt << "nodes\n";
}

Conflict CTNode::getFirstConflict() {
    Conflict con(-1, -1, GridPoint(-1, -1), -1);
    std::vector<int> lengths;
    lengths.resize(solution.size());
    std::transform(solution.begin(), solution.end(), lengths.begin(),
                   [](std::vector<GridPoint> path) { return path.size(); });
    int longestLength = *std::max_element(lengths.begin(), lengths.end());
    std::vector<GridPoint> pointsAtTimei;
    for (int i = 0; i < longestLength; i++) {
        // i is the timeStamp
        pointsAtTimei.clear();
        int agent2 = -1;
        for (std::vector<GridPoint> pathOfOneAgent : solution) {
            agent2++;
            if (i < pathOfOneAgent.size()) {
                if (std::find(pointsAtTimei.begin(), pointsAtTimei.end(),
                              pathOfOneAgent[i]) != pointsAtTimei.end()) {
                    int agent1;
                    for (agent1 = 0; agent1 < pointsAtTimei.size(); agent1++) {
                        if (pointsAtTimei[agent1] == pathOfOneAgent[i]) break;
                    }
                    return Conflict(agent1, agent2, pathOfOneAgent[i], i );
                }
                pointsAtTimei.push_back(pathOfOneAgent[i]);
            }
        }
    }
    return con;
}

void CBS::splitOnConflict(Conflict con, CTNode *node) {
    // split current node into two nodes
    // each has a new set of constraints
    int agent1, agent2;
    agent1 = con.agent1;
    agent2 = con.agent2;
    int conflictTimeStamp = con.timeStamp;
    GridPoint conflictPoint = con.point;
    // generate new contraints;
    Constraint c1(agent1, conflictPoint, conflictTimeStamp);
    Constraint c2(agent2, conflictPoint, conflictTimeStamp);
    std::vector<Constraint> new1;
    std::vector<Constraint> new2;
    // TODO: for each agent, maintain a constraint list
    for (Constraint c : node->constraints)
        if (c.agent == agent1)
            new1.push_back(c);
        else if (c.agent == agent2)
            new2.push_back(c);
    std::cout << "constraints for agent " << agent1 << "\n";
    for (Constraint cc : new1) {
        std::cout << "agent " << agent1 << " point: " << cc.point.x << ","
                  << cc.point.y << "time: " << cc.constraintTimeStamp << "\n";
    }
    std::cout << "constraints for agent " << agent2 << "\n";
    for (Constraint cc : new2) {
        std::cout << "agent " << agent2 << " point: " << cc.point.x << ","
                  << cc.point.y << "time: " << cc.constraintTimeStamp << "\n";
    }
    // check if already in the list, if already in, then no need to proceed
    // TODO: remove redundant code
    std::cout << "original solution\n";
    for (auto path : node->solution) {
        for (GridPoint point : path) {
            std::cout << point.x << "," << point.y << "->";
        }
        std::cout << "finish" << std::endl;
    }

    if (std::find(new1.begin(), new1.end(), c1) == new1.end()) {
        std::cout << "adding constraint "
                  << "agent: " << agent1 << " point: " << c1.point.x << ","
                  << c1.point.y << " time: " << c1.constraintTimeStamp << "\n";
        new1.push_back(c1);
        CTNode *newNode1 =
            new CTNode(node->cost, new1, node->solution, node->costs);
        AStar lowLevelSearchObj1(dimX, dimY, obstacles, new1);
        std::cout << "searching for agent " << agent1
                  << ", start: " << starts[agent1].x << "," << starts[agent1].y
                  << ", goal: " << goals[agent1].x << "," << goals[agent1].y
                  << "\n";
        newNode1->solution[agent1] =
            lowLevelSearchObj1.search(starts[agent1], goals[agent1]);
        Cost cost1 = lowLevelSearchObj1.getFinalCost();
        newNode1->cost -= newNode1->costs[agent1];
        newNode1->cost += cost1;
        newNode1->costs[agent1] = cost1;
        // add children to node
        node->leftChild = newNode1;
        std::cout << "after updating :\n";
        for (auto path : newNode1->solution) {
            for (GridPoint point : path) {
                std::cout << point.x << "," << point.y << "->";
            }
            std::cout << "finish" << std::endl;
        }
    }
    if (std::find(new2.begin(), new2.end(), c2) == new2.end()) {
        std::cout << "adding constraint "
                  << "agent: " << agent2 << " point: " << c2.point.x << ","
                  << c2.point.y << " time: " << c2.constraintTimeStamp << "\n";
        new2.push_back(c2);
        CTNode *newNode2 =
            new CTNode(node->cost, new2, node->solution, node->costs);
        AStar lowLevelSearchObj2(dimX, dimY, obstacles, newNode2->constraints);
        newNode2->solution[agent2] =
            lowLevelSearchObj2.search(starts[agent2], goals[agent2]);
        Cost cost2 = lowLevelSearchObj2.getFinalCost();
        newNode2->cost -= newNode2->costs[agent2];
        newNode2->cost += cost2;
        newNode2->costs[agent2] = cost2;
        node->rightChild = newNode2;
    }
}

void CBS::search() {
    root = new CTNode();
    root->solution.resize(numberOfAgents);
    root->constraints.resize(numberOfAgents);
    // container for single agent constraints
    std::vector<Constraint> cc;
    // container for single agent astar search result
    std::vector<GridPoint> pp;
    // search for each agent, to initialize root ctnode
    root->costs.resize(numberOfAgents);
    for (int i = 0; i < numberOfAgents; i++) {
        // do low level search
        AStar lowLevelSearchObj(dimX, dimY, obstacles, cc);
        pp = lowLevelSearchObj.search(starts[i], goals[i]);
        if (pp.empty()) {
            // no path found initially, just return
            std::cout << "no path found for start : " << starts[i].x << ","
                      << starts[i].y << " goal: " << goals[i].x << ","
                      << goals[i].y;
            return;
        }
        root->cost += lowLevelSearchObj.getFinalCost();
        root->costs[i] = lowLevelSearchObj.getFinalCost();
        root->solution[i] = pp;
    }

    CTNode *currentCTNode = root;
    // search for all the possibilities
    // this guarantees optimality
    std::queue<CTNode *> openSet;
    openSet.push(root);

    Conflict conflict(-1, -1, GridPoint(-1, -1), -1);
    while (!openSet.empty()) {
        currentCTNode = openSet.front();
        openSet.pop();
        onNewNode();
        // test whether finished
        conflict = currentCTNode->getFirstConflict();
        // output conflict
        std::cout << "spliting on conflict \nagent1: " << conflict.agent1
                  << "\nagent2: " << conflict.agent2
                  << "\npoint: " << conflict.point.x << "," << conflict.point.y
                  << "\n"
                  << conflict.timeStamp << "\n";
        if (conflict.agent1 == -1 or conflict.agent2 == -1) {
            // no conflict is found
            solutionNode = currentCTNode;
            return;
        }
        // found a conflict, split the node according to the conflict
        splitOnConflict(conflict, currentCTNode);
        if (currentCTNode->leftChild != nullptr)
            openSet.push(currentCTNode->leftChild);
        if (currentCTNode->rightChild != nullptr)
            openSet.push(currentCTNode->rightChild);
    }
}
