#pragma once

#include <memory>
#include <string>
#include <vector>

#include "entities/CodeEntity.h"

class MethodEntity;
class FieldEntity;

/**
 * @brief Model of C++ class/struct with fields and methods.
 */
class ClassEntity : public CodeEntity {
public:
    /// @brief Creates class entity.
    /// @param name Class name.
    /// @param line Declaration line number.
    /// @param parentClass Optional base class name.
    ClassEntity(std::string name, std::size_t line, std::string parentClass = "");

    /// @brief Adds method member.
    /// @param method Method entity.
    void addMethod(const std::shared_ptr<MethodEntity>& method);
    /// @brief Adds field member.
    /// @param field Field entity.
    void addField(const std::shared_ptr<FieldEntity>& field);
    /// @brief Adds generic child entity.
    /// @param child Child entity.
    void add(const std::shared_ptr<CodeEntity>& child);

    /// @brief Returns parent class name.
    /// @return Parent class name or empty string.
    const std::string& getParentClass() const;
    /// @brief Returns class methods.
    /// @return Method list.
    const std::vector<std::shared_ptr<MethodEntity>>& getMethods() const;
    /// @brief Returns class fields.
    /// @return Field list.
    const std::vector<std::shared_ptr<FieldEntity>>& getFields() const;

    std::string kind() const override;
    std::string renderHtml() const override;

private:
    std::string parentClass_;
    std::vector<std::shared_ptr<MethodEntity>> methods_;
    std::vector<std::shared_ptr<FieldEntity>> fields_;
    std::vector<std::shared_ptr<CodeEntity>> children_;
};
