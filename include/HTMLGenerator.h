#pragma once

#include <string>
#include <vector>

#include "Logger.h"
#include "SourceFile.h"

class HTMLGenerator {
public:
    HTMLGenerator(std::string outputDir, const Logger& logger);
    virtual ~HTMLGenerator() = default;

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
