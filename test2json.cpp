#include "CBS.hpp"
#include "jsonMapUtil.hpp"
void test() {
    CBS cbsObj = readJSONMapFile("testConvert.json");
    cbsObj.search();
    CTNode* solutionNode = cbsObj.getSolutionNode();
    writeSchedule2JSONFile("sampleSchedule.json", solutionNode);
}
int main() {
    test();
    return 0;
}
