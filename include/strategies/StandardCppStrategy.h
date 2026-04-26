#pragma once

#include "strategies/ParsingStrategy.h"

class StandardCppStrategy : public ParsingStrategy {
public:
    bool parseClass(const std::string& line, ClassMatch& out) const override;
    bool parseMethod(const std::string& line, MethodMatch& out) const override;
    bool parseField(const std::string& line, FieldMatch& out) const override;
};
