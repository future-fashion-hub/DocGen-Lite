#pragma once

#include <memory>
#include <string>
#include <vector>

#include "entities/ClassEntity.h"
#include "entities/CodeEntity.h"

class SourceFile {
public:
    SourceFile() = default;
    SourceFile(std::string path, std::string content);

    const std::string& getPath() const;
    const std::string& getContent() const;

    void setPath(const std::string& path);
    void setContent(const std::string& content);

    void addEntity(const std::shared_ptr<CodeEntity>& entity);
    const std::vector<std::shared_ptr<CodeEntity>>& getEntities() const;

private:
    std::string path_;
    std::string content_;
    std::vector<std::shared_ptr<CodeEntity>> entities_;
};

class SourceFileBuilder {
public:
    SourceFileBuilder& setPath(const std::string& path);
    SourceFileBuilder& setContent(const std::string& content);
    SourceFileBuilder& addEntity(const std::shared_ptr<CodeEntity>& entity);
    SourceFile build() const;

private:
    SourceFile file_;
};
