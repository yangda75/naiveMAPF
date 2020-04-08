#include <fstream>
#include "CBS.hpp"
#include "../lib/json.hpp"
#include <direct.h>
#define GetCurrentDir _getcwd
#include <iostream>

std::string get_current_dir() {
    char buff[FILENAME_MAX];  // create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

void print_current_dir() { std::cout << get_current_dir() << std::endl; }

using json = nlohmann::json;


CBS readJSONMapFile(std::string JSONMapFilePath) {
    print_current_dir();
    int dimX{}, dimY{};
    std::vector<GridPoint> obstacles{};
    std::vector<GridPoint> starts{}, goals{};
    int numberOfAgents{};
    std::ifstream i;
    i.open(JSONMapFilePath);
    if (!i) std::cout << "Error opening " << JSONMapFilePath << '\n';
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
    json j_all = json::object();
    json j_all_solutions = json::array();
    for (int i = 0; i < solutionNode->solution.size(); i++) {
        json j = json::object();
	j["agent"] = i;
	j["path"] = json::array();
        for (GridPoint point : solutionNode->solution[i]) {
            j["path"].push_back(json::array({point.x, point.y}));
        }
	j_all_solutions.push_back(j);
    }
    j_all["solutions"] = j_all_solutions;
    std::ofstream o(JSONScheduleFilePath);

    o <<j_all.dump(-1);
}
