#pragma once

#include <string>
#include <vector>

namespace fileutils {

bool directoryExists(const std::string& path);
bool createDirectories(const std::string& path);
std::string joinPath(const std::string& left, const std::string& right);
std::vector<std::string> listFilesRecursive(const std::string& rootPath);

}  // namespace fileutils
