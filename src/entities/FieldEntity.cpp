#include "entities/FieldEntity.h"

#include <utility>

FieldEntity::FieldEntity(std::string name, std::size_t line, std::string fieldType, bool isConst)
    : CodeEntity(std::move(name), line), fieldType_(std::move(fieldType)), isConst_(isConst) {}

void FieldEntity::setType(const std::string& fieldType) {
    fieldType_ = fieldType;
}

const std::string& FieldEntity::getFieldType() const {
    return fieldType_;
}

bool FieldEntity::isConstField() const {
    return isConst_;
}

std::string FieldEntity::kind() const {
    return "field";
}

std::string FieldEntity::renderHtml() const {
    return "<li><b>" + fieldType_ + "</b> " + name_ + "</li>";
}
