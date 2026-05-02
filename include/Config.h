#pragma once

#include <string>
#include <vector>

class Config {
public:
    /// @brief Читает аргументы командной строки и обновляет параметры конфигурации.
    /// @param argc Количество аргументов точки входа.
    /// @param argv Массив аргументов точки входа.
    /// @return true, если переданы оба обязательных пути.
    bool loadFromArgs(int argc, char* argv[]);
    /// @brief Проверяет входные параметры перед генерацией.
    /// @param errorMessage Необязательный текст с деталями ошибки валидации.
    /// @return true, если конфигурация корректна и готова к запуску.
    bool validate(std::string* errorMessage = nullptr) const;

    /// @brief Возвращает путь к входной директории для анализа.
    /// @return Путь входной директории.
    const std::string& inputPath() const;
    /// @brief Возвращает путь к выходной директории документации.
    /// @return Путь выходной директории.
    const std::string& outputPath() const;
    /// @brief Возвращает список исключаемых поддиректорий.
    /// @return Список исключений для сканера.
    const std::vector<std::string>& excludeDirs() const;
    /// @brief Показывает, нужно ли открыть документацию в браузере.
    /// @return true, если включено автоматическое открытие.
    bool shouldOpenAfterGeneration() const;

private:
    std::string inputPath_;
    std::string outputPath_;
    std::vector<std::string> excludeDirs_{".git", "build", "out"};
    bool openAfterGeneration_ = false;
};
