#pragma once

#include <memory>
#include <string>
#include <vector>

#include "entities/ClassEntity.h"
#include "entities/CodeEntity.h"

/**
 * @brief Represents one source file with parsed entities.
 * @note Used as a transport object between scanner, parser and generator.
 */
class SourceFile {
public:
    /// @brief Creates a source file model.
    /// @param path Relative or absolute source path.
    /// @param content Raw file content.
    SourceFile() = default;
    SourceFile(std::string path, std::string content);

    /// @brief Returns source file path.
    /// @return File path string.
    const std::string& getPath() const;
    /// @brief Returns source file content.
    /// @return Raw file content.
    const std::string& getContent() const;

    /// @brief Updates file path.
    /// @param path New file path.
    void setPath(const std::string& path);
    /// @brief Updates raw file content.
    /// @param content New content.
    void setContent(const std::string& content);

    /// @brief Adds parsed entity to this file.
    /// @param entity Parsed code entity.
    void addEntity(const std::shared_ptr<CodeEntity>& entity);
    /// @brief Returns all parsed entities.
    /// @return List of entities.
    const std::vector<std::shared_ptr<CodeEntity>>& getEntities() const;

private:
    std::string path_;
    std::string content_;
    std::vector<std::shared_ptr<CodeEntity>> entities_;
};

/**
 * @brief Helper builder for SourceFile.
 */
class SourceFileBuilder {
public:
    /// @brief Sets path field.
    /// @param path Source file path.
    /// @return Builder instance.
    SourceFileBuilder& setPath(const std::string& path);
    /// @brief Sets content field.
    /// @param content Source content.
    /// @return Builder instance.
    SourceFileBuilder& setContent(const std::string& content);
    /// @brief Adds parsed entity.
    /// @param entity Code entity.
    /// @return Builder instance.
    SourceFileBuilder& addEntity(const std::shared_ptr<CodeEntity>& entity);
    /// @brief Finalizes SourceFile object.
    /// @return Built SourceFile value.
    SourceFile build() const;

private:
    SourceFile file_;
};
