#include "entities/ClassEntity.h"

#include "entities/FieldEntity.h"
#include "entities/MethodEntity.h"

#include <utility>

ClassEntity::ClassEntity(std::string name, std::size_t line, std::string parentClass)
    : CodeEntity(std::move(name), line),
      parentClass_(std::move(parentClass)) {}

void ClassEntity::addMethod(const std::shared_ptr<MethodEntity>& method) {
    methods_.push_back(method);
    add(method);
}

void ClassEntity::addField(const std::shared_ptr<FieldEntity>& field) {
    fields_.push_back(field);
    add(field);
}

void ClassEntity::add(const std::shared_ptr<CodeEntity>& child) {
    children_.push_back(child);
}

const std::string& ClassEntity::getParentClass() const {
    return parentClass_;
}

const std::vector<std::shared_ptr<MethodEntity>>& ClassEntity::getMethods() const {
    return methods_;
}

const std::vector<std::shared_ptr<FieldEntity>>& ClassEntity::getFields() const {
    return fields_;
}

std::string ClassEntity::kind() const {
    return "class";
}

std::string ClassEntity::renderHtml() const {
    std::string html = "<section class='class'><h2>" + name_ + "</h2>";
    if (!parentClass_.empty()) {
        html += "<p>extends: <b>" + parentClass_ + "</b></p>";
    }
    html += "<h3>Members</h3><ul>";
    for (const auto& child : children_) {
        html += child->renderHtml();
    }
    html += "</ul></section>";
    return html;
}
