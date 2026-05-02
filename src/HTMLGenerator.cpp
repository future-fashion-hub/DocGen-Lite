#include "HTMLGenerator.h"

#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <utility>
#include <vector>

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
            case '"':
                out += "&quot;";
                break;
            default:
                out += c;
                break;
        }
    }
    return out;
}

std::string pageHeader(const std::string& title, const std::string& cssHref) {
    return "<!doctype html><html><head><meta charset='utf-8'><meta name='viewport' "
           "content='width=device-width, initial-scale=1'><title>" +
           title + "</title><link rel='stylesheet' href='" + cssHref + "'></head><body>";
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

void cleanupManagedOutput(const std::string& outputDir, const Logger& logger) {
    const std::vector<std::string> generatedEntries = {
        "index.html",
        "navigation.html",
        "classes",
        "files",
        "assets"
    };

    for (std::size_t i = 0; i < generatedEntries.size(); ++i) {
        const std::string path = fileutils::joinPath(outputDir, generatedEntries[i]);
        if (!fileutils::removePathRecursive(path)) {
            logger.warning("Could not clean old documentation artifacts: " + path);
        }
    }
}

void writeSidebar(std::ostream& out,
                  const std::vector<SourceFile>& files,
                  const std::vector<std::shared_ptr<ClassEntity>>& classes,
                  const std::string& prefix) {
    out << "<aside class='sidebar'>";
    out << "<h2>Navigation</h2>";
    out << "<a class='nav-home' href='" << prefix << "index.html'>Project index</a>";
    out << "<input id='searchBox' class='search-box' type='search' placeholder='Search class, file, method...'>";

    out << "<h3>Classes</h3><ul class='nav-list' id='classList'>";
    for (const auto& cls : classes) {
        out << "<li><a href='" << prefix << "classes/" << htmlEscape(cls->getName()) << ".html'>"
            << htmlEscape(cls->getName()) << "</a></li>";
    }
    out << "</ul>";

    out << "<h3>Files</h3><ul class='nav-list' id='fileList'>";
    for (const auto& file : files) {
        out << "<li><a href='" << prefix << "files/" << safeFileName(file.getPath()) << ".html'>"
            << htmlEscape(file.getPath()) << "</a></li>";
    }
    out << "</ul>";
    out << "</aside>";
}

void writeLayoutScript(std::ostream& out) {
    out << "<script>"
           "const q=document.getElementById('searchBox');"
           "if(q){q.addEventListener('input',()=>{"
           "const v=q.value.toLowerCase();"
           "document.querySelectorAll('.nav-list li,.searchable').forEach(el=>{"
           "el.style.display=el.textContent.toLowerCase().includes(v)?'':'none';"
           "});"
           "});}"
           "</script>";
}
}

HTMLGenerator::HTMLGenerator(std::string outputDir, const Logger& logger)
    : outputDir_(std::move(outputDir)), logger_(logger) {}

void HTMLGenerator::generate(const std::vector<SourceFile>& files) const {
    cleanupManagedOutput(outputDir_, logger_);
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

    std::vector<std::shared_ptr<ClassEntity>> classes;
    for (const auto& file : files) {
        for (const auto& entity : file.getEntities()) {
            if (entity->kind() == "class") {
                const auto classEntity = std::dynamic_pointer_cast<ClassEntity>(entity);
                if (classEntity != nullptr) {
                    classes.push_back(classEntity);
                }
            }
        }
    }

    std::ofstream nav(fileutils::joinPath(outputDir_, "navigation.html").c_str());
    nav << pageHeader("Navigation", "./assets/style.css");
    nav << "<div class='layout'>";
    writeSidebar(nav, files, classes, "./");
    nav << "<main class='content'><h1>Navigation</h1>"
        << "<p>Use the left panel to open classes and files.</p></main></div>";
    writeLayoutScript(nav);
    nav << "</body></html>";
}

void HTMLGenerator::generateIndexPage(const std::vector<SourceFile>& files) const {
    std::ofstream output(fileutils::joinPath(outputDir_, "index.html").c_str());

    std::size_t classCount = 0;
    std::size_t functionCount = 0;
    std::vector<std::shared_ptr<ClassEntity>> classes;
    std::vector<std::string> taggedItems;

    for (const auto& file : files) {
        for (const auto& entity : file.getEntities()) {
            if (entity->kind() == "class") {
                ++classCount;
                const auto classEntity = std::dynamic_pointer_cast<ClassEntity>(entity);
                if (classEntity != nullptr) {
                    classes.push_back(classEntity);
                }
            }
            if (entity->kind() == "method") {
                ++functionCount;
            }

            const auto& tags = entity->getComment().getTags();
            if (!tags.empty()) {
                std::ostringstream tagLine;
                tagLine << entity->kind() << " " << entity->getName() << " (" << file.getPath() << "): ";
                bool firstTag = true;
                for (std::map<std::string, std::vector<std::string>>::const_iterator it = tags.begin();
                     it != tags.end();
                     ++it) {
                    if (!firstTag) {
                        tagLine << ", ";
                    }
                    firstTag = false;
                    tagLine << it->first;
                }
                taggedItems.push_back(tagLine.str());
            }
        }
    }

    output << pageHeader("DocGen Lite", "./assets/style.css");
    output << "<div class='layout'>";
    writeSidebar(output, files, classes, "./");

    output << "<main class='content'>";
    output << "<h1>DocGen Lite</h1>";
    output << "<div class='stats'>"
           << "<div class='stat'><span>Files</span><strong>" << files.size() << "</strong></div>"
           << "<div class='stat'><span>Classes</span><strong>" << classCount << "</strong></div>"
           << "<div class='stat'><span>Functions</span><strong>" << functionCount << "</strong></div>"
           << "</div>";

    output << "<section><h2>Class overview</h2><ul>";
    for (const auto& cls : classes) {
        output << "<li class='searchable'><a href='classes/" << htmlEscape(cls->getName()) << ".html'>"
               << htmlEscape(cls->getName()) << "</a></li>";
    }
    output << "</ul></section>";

    output << "<section><h2>Detected @tags</h2>";
    if (taggedItems.empty()) {
        output << "<p>No tagged entities found.</p>";
    } else {
        output << "<ul>";
        for (std::size_t i = 0; i < taggedItems.size(); ++i) {
            output << "<li class='searchable'><code>" << htmlEscape(taggedItems[i]) << "</code></li>";
        }
        output << "</ul>";
    }
    output << "</section>";

    output << "</main></div>";
    writeLayoutScript(output);
    output << "</body></html>";
}

void HTMLGenerator::generateClassPages(const std::vector<SourceFile>& files) const {
    std::vector<std::shared_ptr<ClassEntity>> allClasses;
    for (const auto& file : files) {
        for (const auto& entity : file.getEntities()) {
            if (entity->kind() == "class") {
                const auto classEntity = std::dynamic_pointer_cast<ClassEntity>(entity);
                if (classEntity != nullptr) {
                    allClasses.push_back(classEntity);
                }
            }
        }
    }

    for (const auto& classEntity : allClasses) {
        std::ofstream out(
            fileutils::joinPath(fileutils::joinPath(outputDir_, "classes"), classEntity->getName() + ".html")
                .c_str());
        out << pageHeader("Class " + classEntity->getName(), "../assets/style.css");
        out << "<div class='layout'>";
        writeSidebar(out, files, allClasses, "../");
        out << "<main class='content'>";
        out << "<h1>Class " << htmlEscape(classEntity->getName()) << "</h1>";

        const auto& comment = classEntity->getComment();
        if (!comment.getBrief().empty()) {
            out << "<p class='lead'>" << htmlEscape(comment.getBrief()) << "</p>";
        }
        if (!comment.getDetailed().empty()) {
            out << "<pre>" << htmlEscape(comment.getDetailed()) << "</pre>";
        }

        out << "<h2>Methods</h2><ul>";
        for (const auto& method : classEntity->getMethods()) {
            out << "<li class='searchable'><code>" << htmlEscape(method->getSignature()) << "</code></li>";
        }
        out << "</ul>";

        out << "<h2>Fields</h2><ul>";
        for (const auto& field : classEntity->getFields()) {
            out << "<li class='searchable'><code>" << htmlEscape(field->getFieldType()) << " "
                << htmlEscape(field->getName()) << "</code></li>";
        }
        out << "</ul>";

        out << "</main></div>";
        writeLayoutScript(out);
        out << "</body></html>";
    }
}

void HTMLGenerator::generateFilePages(const std::vector<SourceFile>& files) const {
    std::vector<std::shared_ptr<ClassEntity>> classes;
    for (const auto& file : files) {
        for (const auto& entity : file.getEntities()) {
            if (entity->kind() == "class") {
                const auto classEntity = std::dynamic_pointer_cast<ClassEntity>(entity);
                if (classEntity != nullptr) {
                    classes.push_back(classEntity);
                }
            }
        }
    }

    for (const auto& file : files) {
        std::ofstream out(fileutils::joinPath(fileutils::joinPath(outputDir_, "files"),
                                              safeFileName(file.getPath()) + ".html")
                              .c_str());
        out << pageHeader("File " + file.getPath(), "../assets/style.css");
        out << "<div class='layout'>";
        writeSidebar(out, files, classes, "../");
        out << "<main class='content'>";
        out << "<h1>File " << htmlEscape(file.getPath()) << "</h1>";

        out << "<h2>Entities</h2><ul>";
        for (const auto& entity : file.getEntities()) {
            out << "<li class='searchable'><b>" << entity->kind() << "</b>: " << htmlEscape(entity->getName())
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
                        out << "<li><code>" << htmlEscape(it->first) << "</code> " << htmlEscape(*vit) << "</li>";
                    }
                }
                out << "</ul>";
            }

            out << "</li>";
        }
        out << "</ul></main></div>";
        writeLayoutScript(out);
        out << "</body></html>";
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
