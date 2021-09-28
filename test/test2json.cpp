#include "../src/CBS.hpp"
#include "../src/jsonMapUtil.hpp"

std::string JSONMAPFilePath = "../test/sampleMap.json";

std::string JSONScheduleFilePath = "../test/sampleSchedule.json";


void test() {
    std::cout << "starting test\n";
    CBS cbsObj = readJSONMapFile(JSONMAPFilePath);
    cbsObj.search();
    const std::shared_ptr<CTNode>& solutionNode = cbsObj.getSolutionNode();
    writeSchedule2JSONFile(JSONScheduleFilePath, solutionNode);
}
int main() {
    test();
    return 0;
}
