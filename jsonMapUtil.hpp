#include <fstream>
#include "CBS.hpp"
#include "json.hpp"
using json = nlohmann::json;
CBS readJSONMapFile(std::string JSONMapFilePath) {
    int dimX, dimY;
    std::vector<GridPoint> obstacles;
    std::vector<GridPoint> starts, goals;
    int numberOfAgents;
    std::ifstream i(JSONMapFilePath);
    json j;
    i >> j;
    numberOfAgents = j["numberofagents"];
    dimX = j["dimX"];
    dimY = j["dimY"];

    for (auto obstacle : j["obstacles"]) {
        obstacles.push_back(GridPoint(obstacle[0], obstacle[1]));
    }

    for (auto start : j["starts"]) {
        starts.push_back(GridPoint(start[0], start[1]));
    }
    for (auto goal : j["goals"]) {
        goals.push_back(GridPoint(goal[0], goal[1]));
    }
    CBS cbsObj(numberOfAgents, dimX, dimY, obstacles, starts, goals);
    return cbsObj;
}

void writeSchedule2JSONFile(std::string JSONScheduleFilePath,
                            CTNode* solutionNode) {
    json j = json::object();
    int agent = 0;
    for (auto path : solutionNode->solution) {
      std::string agentStr = std::to_string(agent);
      j[agentStr] = json::array();
        for (GridPoint point : path) {
            std::cout << point.x << "," << point.y << "->";
            j[agentStr].push_back(json::array({point.x, point.y}));
        }
        std::cout << "finish" << std::endl;
        agent++;
    }
    std::ofstream o(JSONScheduleFilePath);

    o << j;
}
