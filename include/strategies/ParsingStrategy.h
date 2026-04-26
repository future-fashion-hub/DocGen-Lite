#pragma once

#include <string>

struct ClassMatch {
    std::string name;
    std::string parent;
};

struct MethodMatch {
    std::string returnType;
    std::string name;
    std::string params;
    bool isStatic;
};

struct FieldMatch {
    std::string type;
    std::string name;
    bool isConst;
};

class ParsingStrategy {
public:
    virtual ~ParsingStrategy() = default;

    virtual bool parseClass(const std::string& line, ClassMatch& out) const = 0;
    virtual bool parseMethod(const std::string& line, MethodMatch& out) const = 0;
    virtual bool parseField(const std::string& line, FieldMatch& out) const = 0;
};
