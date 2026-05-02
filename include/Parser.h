#pragma once

#include <memory>

#include "Observable.h"
#include "SourceFile.h"
#include "strategies/ParsingStrategy.h"

/**
 * @brief Разбирает исходные файлы и извлекает классы, методы, поля и комментарии.
 * @author Команда PKS
 * @version 1.1
 */
class Parser : public Observable {
public:
    /// @brief Создает парсер со стратегией C++ по умолчанию.
    Parser();

    /// @brief Переопределяет стратегию разбора для пользовательских языков.
    /// @param strategy Новый объект стратегии (с передачей владения).
    void setStrategy(std::unique_ptr<ParsingStrategy> strategy);
    /// @brief Разбирает один файл и заполняет его коллекцию сущностей.
    /// @param file Изменяемая модель исходного файла.
    void parseFile(SourceFile& file);
    void notifyGenerationFinished() {
        notify("Documentation generation finished");
    }

private:
    std::shared_ptr<CodeEntity> createEntity(const std::string& line, std::size_t lineNumber) const;

    std::unique_ptr<ParsingStrategy> strategy_;
};
