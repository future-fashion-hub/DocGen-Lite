#pragma once

#include <vector>

#include "Config.h"
#include "Logger.h"
#include "SourceFile.h"

class FileScanner {
public:
    FileScanner(const Config& config, const Logger& logger);

    std::vector<SourceFile> scanDirectory() const;

private:
    bool isSupportedExtension(const std::string& extension) const;
    bool isExcluded(const std::string& path) const;

    const Config& config_;
    const Logger& logger_;
};
