#pragma once

#include <string>
#include <vector>

class Config {
public:
    bool loadFromArgs(int argc, char* argv[]);
    bool validate(std::string* errorMessage = nullptr) const;

    const std::string& inputPath() const;
    const std::string& outputPath() const;
    const std::vector<std::string>& excludeDirs() const;

private:
    std::string inputPath_;
    std::string outputPath_;
    std::vector<std::string> excludeDirs_{".git", "build", "out"};
};
