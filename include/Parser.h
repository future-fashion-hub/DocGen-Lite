#pragma once

#include <memory>

#include "Observable.h"
#include "SourceFile.h"
#include "strategies/ParsingStrategy.h"

class Parser : public Observable {
public:
    Parser();

    void setStrategy(std::unique_ptr<ParsingStrategy> strategy);
    void parseFile(SourceFile& file);
    void notifyGenerationFinished() {
        notify("Documentation generation finished");
    }

private:
    std::shared_ptr<CodeEntity> createEntity(const std::string& line, std::size_t lineNumber) const;

    std::unique_ptr<ParsingStrategy> strategy_;
};
