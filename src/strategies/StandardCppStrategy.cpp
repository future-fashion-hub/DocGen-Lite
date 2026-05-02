#include "strategies/StandardCppStrategy.h"

#include <regex>

bool StandardCppStrategy::parseClass(const std::string& line, ClassMatch& out) const {
    static const std::regex classRegex(
        R"(^\s*(class|struct)\s+([A-Za-z_][A-Za-z0-9_]*)(?:\s*:\s*public\s+([A-Za-z_][A-Za-z0-9_]*))?\s*\{.*)");

    std::smatch match;
    if (!std::regex_match(line, match, classRegex)) {
        return false;
    }

    out.name = match[2].str();
    out.parent = match[3].str();
    return true;
}

bool StandardCppStrategy::parseMethod(const std::string& line, MethodMatch& out) const {
    static const std::regex methodRegex(
        R"(^\s*(static\s+)?([A-Za-z_~][A-Za-z0-9_:<>*&\s]*)\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(([^)]*)\)\s*(const\s*)?[;{].*)");

    std::smatch match;
    if (!std::regex_match(line, match, methodRegex)) {
        return false;
    }

    out.isStatic = !match[1].str().empty();
    out.returnType = match[2].str();
    out.name = match[3].str();
    out.params = match[4].str();
    return true;
}

bool StandardCppStrategy::parseField(const std::string& line, FieldMatch& out) const {
    static const std::regex fieldRegex(
        R"(^\s*(const\s+)?([A-Za-z_][A-Za-z0-9_:<>*&\s]*)\s+([A-Za-z_][A-Za-z0-9_]*)\s*;.*)");

    std::smatch match;
    if (!std::regex_match(line, match, fieldRegex)) {
        return false;
    }

    out.isConst = !match[1].str().empty();
    out.type = match[2].str();
    out.name = match[3].str();
    return true;
}
