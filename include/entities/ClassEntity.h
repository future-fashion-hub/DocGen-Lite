#pragma once

#include <memory>
#include <string>
#include <vector>

#include "entities/CodeEntity.h"

class MethodEntity;
class FieldEntity;

class ClassEntity : public CodeEntity {
public:
    ClassEntity(std::string name, std::size_t line, std::string parentClass = "");

    void addMethod(const std::shared_ptr<MethodEntity>& method);
    void addField(const std::shared_ptr<FieldEntity>& field);
    void add(const std::shared_ptr<CodeEntity>& child);

    const std::string& getParentClass() const;
    const std::vector<std::shared_ptr<MethodEntity>>& getMethods() const;
    const std::vector<std::shared_ptr<FieldEntity>>& getFields() const;

    std::string kind() const override;
    std::string renderHtml() const override;

private:
    std::string parentClass_;
    std::vector<std::shared_ptr<MethodEntity>> methods_;
    std::vector<std::shared_ptr<FieldEntity>> fields_;
    std::vector<std::shared_ptr<CodeEntity>> children_;
};
