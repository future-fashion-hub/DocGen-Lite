#include "entities/Comment.h"

#include <sstream>

namespace {
std::string trim(const std::string& text) {
    const auto begin = text.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
        return "";
    }

    const auto end = text.find_last_not_of(" \t\r\n");
    return text.substr(begin, end - begin + 1);
}

std::string stripCommentPrefix(const std::string& line) {
    std::string cleaned = trim(line);
    if (cleaned.rfind("///", 0) == 0) {
        cleaned = cleaned.substr(3);
    }
    if (cleaned.rfind("/**", 0) == 0) {
        cleaned = cleaned.substr(3);
    }
    if (cleaned.rfind("/*", 0) == 0) {
        cleaned = cleaned.substr(2);
    }
    if (!cleaned.empty() && cleaned.front() == '*') {
        cleaned.erase(cleaned.begin());
    }
    if (cleaned.size() >= 2 && cleaned.substr(cleaned.size() - 2) == "*/") {
        cleaned = cleaned.substr(0, cleaned.size() - 2);
    }
    return trim(cleaned);
}
}

void Comment::parse(const std::string& rawText) {
    brief_.clear();
    detailed_.clear();
    tags_.clear();

    std::stringstream ss(rawText);
    std::string line;
    std::string detailAccumulator;

    while (std::getline(ss, line)) {
        std::string cleaned = stripCommentPrefix(line);
        if (cleaned.empty()) {
            continue;
        }

        if (!cleaned.empty() && cleaned.front() == '@') {
            const auto split = cleaned.find(' ');
            const std::string tag = split == std::string::npos ? cleaned : cleaned.substr(0, split);
            const std::string value = split == std::string::npos ? "" : trim(cleaned.substr(split + 1));
            tags_[tag].push_back(value);
            continue;
        }

        if (brief_.empty()) {
            brief_ = cleaned;
        } else {
            if (!detailAccumulator.empty()) {
                detailAccumulator += '\n';
            }
            detailAccumulator += cleaned;
        }
    }

    detailed_ = detailAccumulator;
}

const std::string& Comment::getBrief() const {
    return brief_;
}

const std::string& Comment::getDetailed() const {
    return detailed_;
}

const std::map<std::string, std::vector<std::string>>& Comment::getTags() const {
    return tags_;
}
