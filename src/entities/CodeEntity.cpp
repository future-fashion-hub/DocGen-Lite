#include "entities/CodeEntity.h"

#include <utility>

CodeEntity::CodeEntity(std::string name, std::size_t line)
    : name_(std::move(name)), line_(line) {}

const std::string& CodeEntity::getName() const {
    return name_;
}

std::size_t CodeEntity::getLine() const {
    return line_;
}

void CodeEntity::setComment(const Comment& comment) {
    comment_ = comment;
}

const Comment& CodeEntity::getComment() const {
    return comment_;
}
