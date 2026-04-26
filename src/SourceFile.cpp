#include "SourceFile.h"

#include <utility>

SourceFile::SourceFile(std::string path, std::string content)
    : path_(std::move(path)), content_(std::move(content)) {}

const std::string& SourceFile::getPath() const {
    return path_;
}

const std::string& SourceFile::getContent() const {
    return content_;
}

void SourceFile::setPath(const std::string& path) {
    path_ = path;
}

void SourceFile::setContent(const std::string& content) {
    content_ = content;
}

void SourceFile::addEntity(const std::shared_ptr<CodeEntity>& entity) {
    entities_.push_back(entity);
}

const std::vector<std::shared_ptr<CodeEntity>>& SourceFile::getEntities() const {
    return entities_;
}

SourceFileBuilder& SourceFileBuilder::setPath(const std::string& path) {
    file_.setPath(path);
    return *this;
}

SourceFileBuilder& SourceFileBuilder::setContent(const std::string& content) {
    file_.setContent(content);
    return *this;
}

SourceFileBuilder& SourceFileBuilder::addEntity(const std::shared_ptr<CodeEntity>& entity) {
    file_.addEntity(entity);
    return *this;
}

SourceFile SourceFileBuilder::build() const {
    return file_;
}
