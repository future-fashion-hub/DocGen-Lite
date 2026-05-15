#include <iostream>
#include <string>

#include "Parser.h"
#include "SourceFile.h"

int main() {
    SourceFile file("scenario_sample.h",
                    "/// @brief Demo class\n"
                    "class Demo {\n"
                    "/// @brief field\n"
                    "int value;\n"
                    "/// @brief sum\n"
                    "/// @param x first\n"
                    "/// @param y second\n"
                    "int sum(int x, int y);\n"
                    "};\n");

    Parser parser;
    parser.parseFile(file);

    std::size_t classes = 0;
    std::size_t methods = 0;
    std::size_t tagged = 0;
    for (const auto& e : file.getEntities()) {
        if (e->kind() == "class") {
            ++classes;
        }
        if (e->kind() == "method") {
            ++methods;
        }
        if (!e->getComment().getTags().empty()) {
            ++tagged;
        }
    }

    std::cout << "SCENARIO RESULT classes=" << classes << " methods=" << methods << " tagged=" << tagged << "\n";
    return 0;
}
