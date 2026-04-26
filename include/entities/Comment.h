#pragma once

#include <map>
#include <string>
#include <vector>

class Comment {
public:
    void parse(const std::string& rawText);

    const std::string& getBrief() const;
    const std::string& getDetailed() const;
    const std::map<std::string, std::vector<std::string>>& getTags() const;

private:
    std::string brief_;
    std::string detailed_;
    std::map<std::string, std::vector<std::string>> tags_;
};
