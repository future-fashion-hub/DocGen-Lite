#include "entities/MethodEntity.h"

#include <sstream>
#include <utility>

MethodEntity::MethodEntity(std::string name, std::size_t line, std::string returnType, bool isStatic)
    : CodeEntity(std::move(name), line),
      returnType_(std::move(returnType)),
      isStatic_(isStatic) {}

void MethodEntity::addParam(const Parameter& param) {
    params_.push_back(param);
}

std::string MethodEntity::getSignature() const {
    std::ostringstream out;
    if (isStatic_) {
        out << "static ";
    }
    out << returnType_ << " " << name_ << "(";
    for (std::size_t i = 0; i < params_.size(); ++i) {
        out << params_[i].getType() << " " << params_[i].getName();
        if (i + 1 < params_.size()) {
            out << ", ";
        }
    }
    out << ")";
    return out.str();
}

const std::string& MethodEntity::getReturnType() const {
    return returnType_;
}

const std::vector<Parameter>& MethodEntity::getParams() const {
    return params_;
}

bool MethodEntity::isStaticMethod() const {
    return isStatic_;
}

std::string MethodEntity::kind() const {
    return "method";
}

std::string MethodEntity::renderHtml() const {
    return "<li><code>" + getSignature() + "</code></li>";
}
