#include "Parser.h"

#include <sstream>
#include <utility>

#include "entities/ClassEntity.h"
#include "entities/FieldEntity.h"
#include "entities/MethodEntity.h"
#include "strategies/StandardCppStrategy.h"

namespace {
std::string trim(const std::string& text) {
    const auto begin = text.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return "";
    }
    const auto end = text.find_last_not_of(" \t\r\n");
    return text.substr(begin, end - begin + 1);
}

void parseMethodParameters(const std::string& rawParams, MethodEntity& method) {
    std::stringstream ss(rawParams);
    std::string part;
    while (std::getline(ss, part, ',')) {
        const std::string token = trim(part);
        if (token.empty() || token == "void") {
            continue;
        }

        const auto split = token.find_last_of(' ');
        if (split == std::string::npos) {
            method.addParam(Parameter(token, ""));
        } else {
            method.addParam(Parameter(trim(token.substr(split + 1)), trim(token.substr(0, split))));
        }
    }
}
}

Parser::Parser()
    : strategy_(std::make_unique<StandardCppStrategy>()) {}

void Parser::setStrategy(std::unique_ptr<ParsingStrategy> strategy) {
    strategy_ = std::move(strategy);
}

void Parser::parseFile(SourceFile& file) {
    notify("Start parsing file: " + file.getPath());

    std::stringstream stream(file.getContent());
    std::string line;
    std::size_t lineNumber = 0;

    std::string pendingComment;
    bool inBlockComment = false;
    std::shared_ptr<ClassEntity> currentClass;

    while (std::getline(stream, line)) {
        ++lineNumber;
        const std::string cleaned = trim(line);

        if (cleaned.rfind("///", 0) == 0) {
            pendingComment += line + "\n";
            continue;
        }

        if (cleaned.find("/**") != std::string::npos) {
            inBlockComment = true;
        }
        if (inBlockComment) {
            pendingComment += line + "\n";
            if (cleaned.find("*/") != std::string::npos) {
                inBlockComment = false;
            }
            continue;
        }

        if (cleaned.empty() || cleaned.rfind("//", 0) == 0) {
            continue;
        }

        if (cleaned == "};" || cleaned == "}") {
            currentClass.reset();
            continue;
        }

        std::shared_ptr<CodeEntity> entity = createEntity(cleaned, lineNumber);
        if (entity == nullptr) {
            continue;
        }

        if (!pendingComment.empty()) {
            Comment comment;
            comment.parse(pendingComment);
            entity->setComment(comment);
            pendingComment.clear();
        }

        if (auto classEntity = std::dynamic_pointer_cast<ClassEntity>(entity)) {
            file.addEntity(classEntity);
            currentClass = classEntity;
            notify("Class detected: " + classEntity->getName());
            continue;
        }

        if (auto methodEntity = std::dynamic_pointer_cast<MethodEntity>(entity)) {
            if (currentClass != nullptr) {
                currentClass->addMethod(methodEntity);
            }
            file.addEntity(methodEntity);
            continue;
        }

        if (auto fieldEntity = std::dynamic_pointer_cast<FieldEntity>(entity)) {
            if (currentClass != nullptr) {
                currentClass->addField(fieldEntity);
            }
            file.addEntity(fieldEntity);
        }
    }

    if (inBlockComment) {
        notify("Warning: unclosed block comment in file: " + file.getPath());
    }

    notify("Finished parsing file: " + file.getPath());
}

std::shared_ptr<CodeEntity> Parser::createEntity(const std::string& line, std::size_t lineNumber) const {
    ClassMatch classMatch;
    if (strategy_->parseClass(line, classMatch)) {
        return std::make_shared<ClassEntity>(classMatch.name, lineNumber, classMatch.parent);
    }

    MethodMatch methodMatch;
    if (strategy_->parseMethod(line, methodMatch)) {
        auto method = std::make_shared<MethodEntity>(
            trim(methodMatch.name),
            lineNumber,
            trim(methodMatch.returnType),
            methodMatch.isStatic);
        parseMethodParameters(methodMatch.params, *method);
        return method;
    }

    FieldMatch fieldMatch;
    if (strategy_->parseField(line, fieldMatch)) {
        return std::make_shared<FieldEntity>(
            trim(fieldMatch.name),
            lineNumber,
            trim(fieldMatch.type),
            fieldMatch.isConst);
    }

    return nullptr;
}
