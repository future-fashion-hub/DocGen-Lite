#include <iostream>
#include <memory>
#include <string>

#include "HTMLGenerator.h"
#include "Logger.h"
#include "SourceFile.h"
#include "entities/ClassEntity.h"
#include "entities/MethodEntity.h"

int main() {
    Logger logger;
    const std::string outDir = "./build/scenario_html";
    HTMLGenerator generator(outDir, logger);

    SourceFile file("scenario.h", "");
    auto cls = std::make_shared<ClassEntity>("ScenarioClass", 1);
    auto method = std::make_shared<MethodEntity>("run", 2, "void", false);
    cls->addMethod(method);
    file.addEntity(cls);
    file.addEntity(method);

    generator.generate({file});
    std::cout << "SCENARIO OK: docs generated in " << outDir << "\n";
    return 0;
}
