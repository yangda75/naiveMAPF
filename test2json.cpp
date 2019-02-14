#include "CBS.hpp"
#include "jsonMapUtil.hpp"
void test() {
    CBS cbsObj = readJSONMapFile("sampleMap.json");
    cbsObj.search();
    CTNode* solutionNode = cbsObj.getSolutionNode();
    writeSchedule2JSONFile("sampleSchedule.json", solutionNode);
    for (auto path : solutionNode->solution) {
        for (GridPoint point : path) {
            std::cout << point.x << "," << point.y << "->";
        }
        std::cout << "finish" << std::endl;
    }
}
int main() {
    test();
    return 0;
}
