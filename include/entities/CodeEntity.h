#pragma once

#include <string>

#include "entities/Comment.h"

class CodeEntity {
public:
    CodeEntity(std::string name, std::size_t line);
    virtual ~CodeEntity() = default;

    const std::string& getName() const;
    std::size_t getLine() const;

    void setComment(const Comment& comment);
    const Comment& getComment() const;

    virtual std::string kind() const = 0;
    virtual std::string renderHtml() const = 0;

protected:
    std::string name_;
    std::size_t line_;
    Comment comment_;
};
