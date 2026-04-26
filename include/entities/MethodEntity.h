#pragma once

#include <string>
#include <vector>

#include "entities/CodeEntity.h"
#include "entities/Parameter.h"

class MethodEntity : public CodeEntity {
public:
    MethodEntity(std::string name, std::size_t line, std::string returnType, bool isStatic);

    void addParam(const Parameter& param);
    std::string getSignature() const;

    const std::string& getReturnType() const;
    const std::vector<Parameter>& getParams() const;
    bool isStaticMethod() const;

    std::string kind() const override;
    std::string renderHtml() const override;

private:
    std::string returnType_;
    std::vector<Parameter> params_;
    bool isStatic_;
};
