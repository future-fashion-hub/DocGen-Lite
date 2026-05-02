#pragma once

#include <vector>

#include "Config.h"
#include "Logger.h"
#include "SourceFile.h"

class FileScanner {
public:
    /// @brief Создает сканер для рекурсивного поиска файлов.
    /// @param config Рабочая конфигурация с путями и исключениями.
    /// @param logger Логгер для прогресса и предупреждений.
    FileScanner(const Config& config, const Logger& logger);

    /// @brief Сканирует директорию и возвращает поддерживаемые исходные файлы.
    /// @return Набор исходных файлов с загруженным содержимым.
    std::vector<SourceFile> scanDirectory() const;

private:
    /// @brief Проверяет, относится ли расширение к поддерживаемым C/C++ форматам.
    /// @param extension Расширение файла вместе с точкой.
    /// @return true для поддерживаемых расширений.
    bool isSupportedExtension(const std::string& extension) const;
    /// @brief Применяет фильтр исключений по пути.
    /// @param path Путь кандидата.
    /// @return true, если файл нужно пропустить.
    bool isExcluded(const std::string& path) const;

    const Config& config_;
    const Logger& logger_;
};
