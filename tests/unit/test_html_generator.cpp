#include <catch2/catch.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "HTMLGenerator.h"
#include "Logger.h"
#include "entities/ClassEntity.h"
#include "entities/Comment.h"
#include "entities/MethodEntity.h"
#include "test_helpers.h"

TEST_CASE("Logger prints level labels") {
    std::stringstream buffer;
    auto* old = std::cout.rdbuf(buffer.rdbuf());
    Logger logger(Logger::LogLevel::Info);
    logger.info("i");
    logger.warning("w");
    logger.error("e");
    std::cout.rdbuf(old);
    const std::string text = buffer.str();
    CHECK(text.find("[INFO] i") != std::string::npos);
    CHECK(text.find("[WARNING] w") != std::string::npos);
    CHECK(text.find("[ERROR] e") != std::string::npos);
}

TEST_CASE("Logger respects minimum level") {
    std::stringstream buffer;
    auto* old = std::cout.rdbuf(buffer.rdbuf());
    Logger logger(Logger::LogLevel::Warning);
    logger.info("hidden");
    logger.warning("shown");
    std::cout.rdbuf(old);
    const std::string text = buffer.str();
    CHECK(text.find("hidden") == std::string::npos);
    CHECK(text.find("shown") != std::string::npos);
}

TEST_CASE("HTMLGenerator creates base pages") {
    std::string outDir = testTempDir("html_out1");
    Logger logger;
    HTMLGenerator generator(outDir, logger);

    SourceFile file("a.h", "");
    file.addEntity(std::make_shared<ClassEntity>("A", 1));
    generator.generate({file});

    std::ifstream index(fileutils::joinPath(outDir, "index.html").c_str());
    std::ifstream nav(fileutils::joinPath(outDir, "navigation.html").c_str());
    CHECK(index.good());
    CHECK(nav.good());
}

TEST_CASE("HTMLGenerator renders detected tags in index") {
    std::string outDir = testTempDir("html_out2");
    Logger logger;
    HTMLGenerator generator(outDir, logger);

    SourceFile file("a.h", "");
    auto method = std::make_shared<MethodEntity>("run", 2, "int", false);
    Comment c;
    c.parse("/// @brief x\n/// @param p test");
    method->setComment(c);
    file.addEntity(method);
    generator.generate({file});

    std::ifstream index(fileutils::joinPath(outDir, "index.html").c_str());
    std::stringstream s;
    s << index.rdbuf();
    const std::string html = s.str();
    CHECK(html.find("Detected @tags") != std::string::npos);
    CHECK(html.find("@param") != std::string::npos);
}

TEST_CASE("HTMLGenerator cleans previous generated files") {
    std::string outDir = testTempDir("html_out3");
    fileutils::createDirectories(fileutils::joinPath(outDir, "classes"));
    writeTextFile(fileutils::joinPath(outDir, "classes/old.html"), "old");

    Logger logger;
    HTMLGenerator generator(outDir, logger);
    SourceFile file("a.h", "");
    generator.generate({file});

    std::ifstream oldFile(fileutils::joinPath(outDir, "classes/old.html").c_str());
    CHECK_FALSE(oldFile.good());
}

TEST_CASE("HTMLGenerator copies style asset") {
    std::string outDir = testTempDir("html_out4");
    Logger logger;
    HTMLGenerator generator(outDir, logger);
    SourceFile file("a.h", "");
    generator.generate({file});
    std::ifstream css(fileutils::joinPath(outDir, "assets/style.css").c_str());
    CHECK(css.good());
}
