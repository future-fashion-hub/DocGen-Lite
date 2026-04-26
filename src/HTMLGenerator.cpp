#include "HTMLGenerator.h"

#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <utility>

#include "FileUtils.h"

#include "entities/ClassEntity.h"
#include "entities/FieldEntity.h"
#include "entities/MethodEntity.h"

namespace {
std::string safeFileName(std::string path) {
    for (char& c : path) {
        const unsigned char uc = static_cast<unsigned char>(c);
        if (!std::isalnum(uc) && c != '_' && c != '-') {
            c = '_';
        }
    }
    if (path.empty()) {
        return "file";
    }
    return path;
}

std::string htmlEscape(const std::string& value) {
    std::string out;
    out.reserve(value.size());
    for (char c : value) {
        switch (c) {
            case '&':
                out += "&amp;";
                break;
            case '<':
                out += "&lt;";
                break;
            case '>':
                out += "&gt;";
                break;
            default:
                out += c;
                break;
        }
    }
    return out;
}

std::string pageHeader(const std::string& title) {
    return "<!doctype html><html><head><meta charset='utf-8'><title>" + title +
           "</title><link rel='stylesheet' href='../assets/style.css'></head><body><main>";
}

bool copyFileBinary(const std::string& from, const std::string& to) {
    std::ifstream src(from.c_str(), std::ios::binary);
    if (!src) {
        return false;
    }

    std::ofstream dst(to.c_str(), std::ios::binary);
    if (!dst) {
        return false;
    }

    dst << src.rdbuf();
    return dst.good();
}
}

HTMLGenerator::HTMLGenerator(std::string outputDir, const Logger& logger)
    : outputDir_(std::move(outputDir)), logger_(logger) {}

void HTMLGenerator::generate(const std::vector<SourceFile>& files) const {
    buildNavigation(files);
    generateIndexPage(files);
    generateClassPages(files);
    generateFilePages(files);
    copyAssets();

    logger_.info("HTML documentation generated in: " + outputDir_);
}

void HTMLGenerator::buildNavigation(const std::vector<SourceFile>& files) const {
    fileutils::createDirectories(outputDir_);
    fileutils::createDirectories(fileutils::joinPath(outputDir_, "classes"));
    fileutils::createDirectories(fileutils::joinPath(outputDir_, "files"));

    std::ofstream nav(fileutils::joinPath(outputDir_, "navigation.html").c_str());
    nav << "<!doctype html><html><head><meta charset='utf-8'><title>Navigation</title>"
        << "<link rel='stylesheet' href='./assets/style.css'></head><body><main>";
    nav << "<h2>Navigation</h2>";
    nav << "<p><a href='./index.html'>Index</a></p>";
    nav << "<h3>Files</h3><ul>";
    for (const auto& file : files) {
        nav << "<li><a href='files/" << safeFileName(file.getPath()) << ".html'>"
            << htmlEscape(file.getPath()) << "</a></li>";
    }
    nav << "</ul></main></body></html>";
}

void HTMLGenerator::generateIndexPage(const std::vector<SourceFile>& files) const {
    std::ofstream output(fileutils::joinPath(outputDir_, "index.html").c_str());

    std::size_t classCount = 0;
    std::size_t functionCount = 0;
    for (const auto& file : files) {
        for (const auto& entity : file.getEntities()) {
            if (entity->kind() == "class") {
                ++classCount;
            }
            if (entity->kind() == "method") {
                ++functionCount;
            }
        }
    }

    output << "<!doctype html><html><head><meta charset='utf-8'><title>DocGen Lite</title>"
           << "<link rel='stylesheet' href='./assets/style.css'></head><body><main>";
    output << "<h1>DocGen Lite</h1>";
    output << "<p>Processed files: <b>" << files.size() << "</b></p>";
    output << "<p>Found classes: <b>" << classCount << "</b></p>";
    output << "<p>Found functions: <b>" << functionCount << "</b></p>";

    output << "<h2>Files</h2><ul>";
    for (const auto& file : files) {
        output << "<li><a href='files/" << safeFileName(file.getPath()) << ".html'>"
               << htmlEscape(file.getPath()) << "</a></li>";
    }
    output << "</ul>";
    output << "</main></body></html>";
}

void HTMLGenerator::generateClassPages(const std::vector<SourceFile>& files) const {
    for (const auto& file : files) {
        for (const auto& entity : file.getEntities()) {
            if (entity->kind() != "class") {
                continue;
            }

            const auto classEntity = std::dynamic_pointer_cast<ClassEntity>(entity);
            if (classEntity == nullptr) {
                continue;
            }

            std::ofstream out(
                fileutils::joinPath(fileutils::joinPath(outputDir_, "classes"),
                                    classEntity->getName() + ".html")
                    .c_str());
            out << pageHeader("Class " + classEntity->getName());
            out << "<a href='../index.html'>Back to index</a>";
            out << "<h1>Class " << htmlEscape(classEntity->getName()) << "</h1>";

            const auto& comment = classEntity->getComment();
            if (!comment.getBrief().empty()) {
                out << "<p>" << htmlEscape(comment.getBrief()) << "</p>";
            }
            if (!comment.getDetailed().empty()) {
                out << "<pre>" << htmlEscape(comment.getDetailed()) << "</pre>";
            }

            out << "<h2>Methods</h2><ul>";
            for (const auto& method : classEntity->getMethods()) {
                out << "<li><code>" << htmlEscape(method->getSignature()) << "</code></li>";
            }
            out << "</ul>";

            out << "<h2>Fields</h2><ul>";
            for (const auto& field : classEntity->getFields()) {
                out << "<li><code>" << htmlEscape(field->getFieldType()) << " "
                    << htmlEscape(field->getName()) << "</code></li>";
            }
            out << "</ul>";

            out << "</main></body></html>";
        }
    }
}

void HTMLGenerator::generateFilePages(const std::vector<SourceFile>& files) const {
    for (const auto& file : files) {
        std::ofstream out(
            fileutils::joinPath(fileutils::joinPath(outputDir_, "files"),
                    safeFileName(file.getPath()) + ".html")
            .c_str());
        out << pageHeader("File " + file.getPath());
        out << "<a href='../index.html'>Back to index</a>";
        out << "<h1>File " << htmlEscape(file.getPath()) << "</h1>";

        out << "<h2>Entities</h2><ul>";
        for (const auto& entity : file.getEntities()) {
            out << "<li><b>" << entity->kind() << "</b>: " << htmlEscape(entity->getName())
                << " (line " << entity->getLine() << ")";

            const auto& comment = entity->getComment();
            if (!comment.getBrief().empty()) {
                out << "<br><small>" << htmlEscape(comment.getBrief()) << "</small>";
            }

            const auto& tags = comment.getTags();
            if (!tags.empty()) {
                out << "<ul>";
                for (std::map<std::string, std::vector<std::string>>::const_iterator it = tags.begin();
                     it != tags.end();
                     ++it) {
                    for (std::vector<std::string>::const_iterator vit = it->second.begin();
                         vit != it->second.end();
                         ++vit) {
                        out << "<li><code>" << htmlEscape(it->first) << "</code> "
                            << htmlEscape(*vit)
                            << "</li>";
                    }
                }
                out << "</ul>";
            }

            out << "</li>";
        }
        out << "</ul></main></body></html>";
    }
}

void HTMLGenerator::copyAssets() const {
    const std::string assetsDir = fileutils::joinPath(outputDir_, "assets");
    if (!fileutils::createDirectories(assetsDir)) {
        logger_.warning("Unable to create assets directory: " + assetsDir);
        return;
    }

    const std::string sourceStyle = fileutils::joinPath("assets", "style.css");
    const std::string targetStyle = fileutils::joinPath(assetsDir, "style.css");

    if (copyFileBinary(sourceStyle, targetStyle)) {
        return;
    }

    logger_.warning("Unable to copy assets/style.css. Writing fallback stylesheet.");
    std::ofstream fallback(targetStyle.c_str());
    fallback << "body{font-family:Arial,sans-serif;margin:0;padding:24px;}"
                "main{max-width:960px;margin:0 auto;}"
                "code{background:#f3f3f3;padding:2px 4px;border-radius:4px;}";
}
