#pragma once

#include <ctime>
#include <fstream>
#include <string>

#include "FileUtils.h"

inline std::string testTempDir(const std::string& name) {
    const std::string dir = fileutils::joinPath("tmp_tests", name + "_" + std::to_string(std::time(nullptr)));
    fileutils::createDirectories(dir);
    return dir;
}

inline void writeTextFile(const std::string& path, const std::string& content) {
    std::ofstream out(path.c_str(), std::ios::binary);
    out << content;
}
