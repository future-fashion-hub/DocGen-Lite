#include "FileUtils.h"

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#endif

namespace {

bool isDirectory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

bool makeDirectory(const std::string& path) {
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0 || errno == EEXIST;
#else
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
#endif
}

std::string normalize(std::string path) {
    std::replace(path.begin(), path.end(), '\\', '/');
    return path;
}

}

namespace fileutils {

bool directoryExists(const std::string& path) {
    return isDirectory(path);
}

bool createDirectories(const std::string& path) {
    if (path.empty()) {
        return false;
    }

    std::string normalized = normalize(path);
    if (directoryExists(normalized)) {
        return true;
    }

    std::string current;
    for (std::size_t i = 0; i < normalized.size(); ++i) {
        const char c = normalized[i];
        current.push_back(c);
        if (c == '/' || i + 1 == normalized.size()) {
            if (current.size() == 1 && current[0] == '/') {
                continue;
            }
            if (current.size() == 3 && current[1] == ':' && current[2] == '/') {
                continue;
            }
            if (!directoryExists(current) && !makeDirectory(current)) {
                return false;
            }
        }
    }

    return true;
}

std::string joinPath(const std::string& left, const std::string& right) {
    if (left.empty()) {
        return right;
    }
    if (right.empty()) {
        return left;
    }

    const bool leftEnds = left.back() == '/' || left.back() == '\\';
    const bool rightStarts = right.front() == '/' || right.front() == '\\';

    if (leftEnds && rightStarts) {
        return left + right.substr(1);
    }
    if (!leftEnds && !rightStarts) {
        return left + "/" + right;
    }
    return left + right;
}

std::vector<std::string> listFilesRecursive(const std::string& rootPath) {
    std::vector<std::string> files;
    std::vector<std::string> stack;
    stack.push_back(rootPath);

    while (!stack.empty()) {
        const std::string current = stack.back();
        stack.pop_back();

        DIR* dir = opendir(current.c_str());
        if (dir == nullptr) {
            continue;
        }

        struct dirent* entry = nullptr;
        while ((entry = readdir(dir)) != nullptr) {
            const std::string name = entry->d_name;
            if (name == "." || name == "..") {
                continue;
            }

            const std::string fullPath = joinPath(current, name);
            if (isDirectory(fullPath)) {
                stack.push_back(fullPath);
            } else {
                files.push_back(fullPath);
            }
        }

        closedir(dir);
    }

    return files;
}

}  // namespace fileutils
