#pragma once

#include <string>
#include <vector>

#include "Logger.h"
#include "SourceFile.h"

/**
 * @brief Генерирует HTML-страницы документации из разобранных моделей исходников.
 * @note Использует скопированные CSS-ресурсы для единого визуального стиля.
 */
class HTMLGenerator {
public:
    /// @brief Создает генератор с целевой выходной директорией.
    /// @param outputDir Директория, в которую записывается документация.
    /// @param logger Логгер для прогресса и предупреждений.
    HTMLGenerator(std::string outputDir, const Logger& logger);
    virtual ~HTMLGenerator() = default;

    /// @brief Генерирует полный статический сайт документации.
    /// @param files Разобранные модели исходных файлов.
    void generate(const std::vector<SourceFile>& files) const;

protected:
    virtual void buildNavigation(const std::vector<SourceFile>& files) const;
    virtual void generateIndexPage(const std::vector<SourceFile>& files) const;
    virtual void generateClassPages(const std::vector<SourceFile>& files) const;
    virtual void generateFilePages(const std::vector<SourceFile>& files) const;
    virtual void copyAssets() const;

    std::string outputDir_;
    const Logger& logger_;
};
