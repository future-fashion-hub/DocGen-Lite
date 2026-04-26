#include "FileScanner.h"

#include <fstream>
#include <sstream>

#include "FileUtils.h"

namespace {
std::string extensionOf(const std::string& path) {
    const auto slashPos = path.find_last_of("/\\");
    const auto dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos) {
        return "";
    }
    if (slashPos != std::string::npos && dotPos < slashPos) {
        return "";
    }
    return path.substr(dotPos);
}
}

FileScanner::FileScanner(const Config& config, const Logger& logger)
    : config_(config), logger_(logger) {}

std::vector<SourceFile> FileScanner::scanDirectory() const {
    std::vector<SourceFile> files;
    if (!fileutils::directoryExists(config_.inputPath())) {
        logger_.warning("Input directory does not exist: " + config_.inputPath());
        return files;
    }

    const auto paths = fileutils::listFilesRecursive(config_.inputPath());
    for (const auto& pathText : paths) {
        if (isExcluded(pathText)) {
            continue;
        }

        if (!isSupportedExtension(extensionOf(pathText))) {
            continue;
        }

        std::ifstream input(pathText.c_str(), std::ios::binary);
        if (!input) {
            logger_.warning("Unable to read file: " + pathText);
            continue;
        }

        std::ostringstream buffer;
        buffer << input.rdbuf();

        SourceFileBuilder builder;
        files.push_back(builder.setPath(pathText).setContent(buffer.str()).build());
    }

    return files;
}

bool FileScanner::isSupportedExtension(const std::string& extension) const {
    return extension == ".cpp" || extension == ".h" || extension == ".hpp" || extension == ".c" || extension == ".cc";
}

bool FileScanner::isExcluded(const std::string& path) const {
    for (const auto& excluded : config_.excludeDirs()) {
        if (!excluded.empty() && path.find(excluded) != std::string::npos) {
            return true;
        }
    }
    return false;
}
