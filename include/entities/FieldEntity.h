#pragma once

#include <string>

#include "entities/CodeEntity.h"

class FieldEntity : public CodeEntity {
public:
    FieldEntity(std::string name, std::size_t line, std::string fieldType, bool isConst);

    void setType(const std::string& fieldType);

    const std::string& getFieldType() const;
    bool isConstField() const;

    std::string kind() const override;
    std::string renderHtml() const override;

private:
    std::string fieldType_;
    bool isConst_;
};
