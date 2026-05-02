#include "Config.h"

#include <sstream>

#include "FileUtils.h"

bool Config::loadFromArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            inputPath_ = argv[++i];
        } else if ((arg == "-o" || arg == "--output") && i + 1 < argc) {
            outputPath_ = argv[++i];
        } else if (arg == "--exclude" && i + 1 < argc) {
            excludeDirs_.clear();
            std::stringstream ss(argv[++i]);
            std::string item;
            while (std::getline(ss, item, ',')) {
                if (!item.empty()) {
                    excludeDirs_.push_back(item);
                }
            }
        } else if (arg == "--open") {
            openAfterGeneration_ = true;
        }
    }

    return !inputPath_.empty() && !outputPath_.empty();
}

bool Config::validate(std::string* errorMessage) const {
    if (inputPath_.empty() || outputPath_.empty()) {
        if (errorMessage != nullptr) {
            *errorMessage = "Both input and output paths must be set.";
        }
        return false;
    }

    if (!fileutils::directoryExists(inputPath_)) {
        if (errorMessage != nullptr) {
            *errorMessage = "Input path does not exist or is not a directory: " + inputPath_;
        }
        return false;
    }

    return true;
}

const std::string& Config::inputPath() const {
    return inputPath_;
}

const std::string& Config::outputPath() const {
    return outputPath_;
}

const std::vector<std::string>& Config::excludeDirs() const {
    return excludeDirs_;
}

bool Config::shouldOpenAfterGeneration() const {
    return openAfterGeneration_;
}
