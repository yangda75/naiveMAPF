#include <direct.h>
#include <fstream>
#include "../lib/json.hpp"
#include "CBS.hpp"
#include <iostream>

std::string get_current_dir() {
    char buff[FILENAME_MAX];  // create string buffer to hold path
    _getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

void print_current_dir() { std::cout << get_current_dir() << std::endl; }

using json = nlohmann::json;

CBS readJSONMapFile(const std::string& JSONMapFilePath) {
    print_current_dir();
    std::vector<GridPoint> obstacles{};
    std::vector<GridPoint> starts{}, goals{};
    std::ifstream i;
    i.open(JSONMapFilePath);
    if (!i) std::cout << "Error opening " << JSONMapFilePath << '\n';
    json j;
    i >> j;
    int numberOfAgents = j["numberofagents"];
    int dimX = j["dimX"];
    int dimY = j["dimY"];

    for (auto obstacle : j["obstacles"]) {
        obstacles.emplace_back(obstacle[0], obstacle[1]);
    }

    for (auto start : j["starts"]) {
        starts.emplace_back(start[0], start[1]);
    }
    for (auto goal : j["goals"]) {
        goals.emplace_back(goal[0], goal[1]);
    }
    CBS cbsObj(numberOfAgents, dimX, dimY, obstacles, starts, goals);
    return cbsObj;
}

void writeSchedule2JSONFile(const std::string& JSONScheduleFilePath,
                            const std::shared_ptr<CTNode>& solutionNode) {
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

    o << j_all.dump(-1);
}
