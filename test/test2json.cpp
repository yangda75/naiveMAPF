#include "../src/CBS.hpp"
#include "../src/jsonMapUtil.hpp"

std::string JSONMAPFilePath =
    "D:\\Documents\\seer_proj\\naiveMAPF\\test\\sampleMap.json";

std::string JSONScheduleFilePath = 
    "D:\\Documents\\seer_proj\\naiveMAPF\\test\\sampleSchedule.json";


void test() {
    std::cout << "starting test\n";
    CBS cbsObj = readJSONMapFile(JSONMAPFilePath);
    cbsObj.search();
    CTNode* solutionNode = cbsObj.getSolutionNode();
    writeSchedule2JSONFile(JSONScheduleFilePath, solutionNode);
}
int main() {
    test();
    return 0;
}
