#include <iostream>
#include <string>
#include <vector>

#include "Config.h"

int main() {
    Config config;
    std::string input = ".";
    std::string output = "./build/scenario_docs";
    std::string exclude = ".git,build,out";
    std::string open = "--open";
    char arg0[] = "docgen";
    std::string iFlag = "-i";
    std::string oFlag = "-o";
    std::string exFlag = "--exclude";
    char* argv[] = {
        arg0, &iFlag[0], &input[0], &oFlag[0], &output[0], &exFlag[0], &exclude[0], &open[0]
    };

    if (!config.loadFromArgs(8, argv)) {
        std::cout << "SCENARIO FAIL: args not parsed\n";
        return 1;
    }
    std::cout << "SCENARIO OK: args parsed, output=" << config.outputPath() << "\n";
    return 0;
}
