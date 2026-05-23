#pragma once

#include <string>

#include "entities/CodeEntity.h"

/**
 * @brief Model of field/variable declaration.
 */
class FieldEntity : public CodeEntity {
public:
    /// @brief Creates field entity.
    /// @param name Field name.
    /// @param line Declaration line number.
    /// @param fieldType Field type text.
    /// @param isConst Const qualifier flag.
    FieldEntity(std::string name, std::size_t line, std::string fieldType, bool isConst);

    /// @brief Updates field type.
    /// @param fieldType New type text.
    void setType(const std::string& fieldType);

    /// @brief Returns field type.
    /// @return Type text.
    const std::string& getFieldType() const;
    /// @brief Returns const qualifier.
    /// @return true if field is const.
    bool isConstField() const;

    std::string kind() const override;
    std::string renderHtml() const override;

private:
    std::string fieldType_;
    bool isConst_;
};
