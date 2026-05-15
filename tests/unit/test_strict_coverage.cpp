#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

#include "Config.h"
#include "FileUtils.h"
#include "Logger.h"
#include "Parser.h"
#include "SourceFile.h"
#include "entities/Comment.h"
#include "entities/FieldEntity.h"
#include "entities/MethodEntity.h"
#include "test_helpers.h"

TEST_CASE("Config loadFromArgs supports mixed order") {
    Config c;
    std::string in = testTempDir("cfg_mixed_in");
    std::string out = testTempDir("cfg_mixed_out");
    char arg0[] = "docgen";
    std::string a1 = "-o", a2 = out, a3 = "-i", a4 = in;
    char* argv[] = {arg0, &a1[0], &a2[0], &a3[0], &a4[0]};
    REQUIRE(c.loadFromArgs(5, argv));
    CHECK(c.inputPath() == in);
    CHECK(c.outputPath() == out);
}

TEST_CASE("Config loadFromArgs ignores unknown args") {
    Config c;
    std::string in = testTempDir("cfg_unknown_in");
    std::string out = testTempDir("cfg_unknown_out");
    char arg0[] = "docgen";
    std::string a1 = "--foo", a2 = "bar", a3 = "-i", a4 = in, a5 = "-o", a6 = out;
    char* argv[] = {arg0, &a1[0], &a2[0], &a3[0], &a4[0], &a5[0], &a6[0]};
    REQUIRE(c.loadFromArgs(7, argv));
}

TEST_CASE("Config validate fails when output empty") {
    Config c;
    std::string in = testTempDir("cfg_no_out");
    char arg0[] = "docgen";
    std::string a1 = "-i", a2 = in;
    char* argv[] = {arg0, &a1[0], &a2[0]};
    CHECK_FALSE(c.loadFromArgs(3, argv));
}

TEST_CASE("Config getters return parsed values") {
    Config c;
    std::string in = testTempDir("cfg_get_in");
    std::string out = testTempDir("cfg_get_out");
    char arg0[] = "docgen";
    std::string a1 = "-i", a2 = in, a3 = "-o", a4 = out, a5 = "--exclude", a6 = "x,y";
    char* argv[] = {arg0, &a1[0], &a2[0], &a3[0], &a4[0], &a5[0], &a6[0]};
    REQUIRE(c.loadFromArgs(7, argv));
    CHECK(c.inputPath() == in);
    CHECK(c.outputPath() == out);
    CHECK(c.excludeDirs().size() == 2u);
}

TEST_CASE("FileUtils createDirectories supports nested folders") {
    std::string base = testTempDir("fu_nested");
    std::string nested = fileutils::joinPath(base, "a/b/c");
    CHECK(fileutils::createDirectories(nested));
    CHECK(fileutils::directoryExists(nested));
}

TEST_CASE("FileUtils joinPath keeps already separated boundaries") {
    CHECK(fileutils::joinPath("a/", "/b") == "a/b");
}

TEST_CASE("Comment parse handles empty input") {
    Comment c;
    c.parse("");
    CHECK(c.getBrief().empty());
    CHECK(c.getDetailed().empty());
    CHECK(c.getTags().empty());
}

TEST_CASE("Comment parse handles only tags") {
    Comment c;
    c.parse("/// @param x x\n/// @return ok\n");
    CHECK(c.getBrief().empty());
    CHECK(c.getTags().count("@param") == 1u);
    CHECK(c.getTags().count("@return") == 1u);
}

TEST_CASE("MethodEntity renderHtml contains signature") {
    MethodEntity m("go", 1, "void", false);
    m.addParam(Parameter("x", "int"));
    CHECK(m.renderHtml().find("go") != std::string::npos);
}

TEST_CASE("MethodEntity return type getter works") {
    MethodEntity m("go", 1, "Result", false);
    CHECK(m.getReturnType() == "Result");
}

TEST_CASE("FieldEntity renderHtml reflects setType") {
    FieldEntity f("n", 3, "int", false);
    f.setType("size_t");
    CHECK(f.renderHtml().find("size_t") != std::string::npos);
}

TEST_CASE("Parser handles file with no entities") {
    Parser p;
    SourceFile f("empty.h", "   \n// just comment\n");
    p.parseFile(f);
    CHECK(f.getEntities().empty());
}

TEST_CASE("Parser handles unclosed block comment without crash") {
    Parser p;
    SourceFile f("broken.h", "/**\n * broken\nint x;\n");
    p.parseFile(f);
    CHECK(f.getEntities().empty());
}

TEST_CASE("Parser handles void parameter list") {
    Parser p;
    SourceFile f("v.h", "int run(void);\n");
    p.parseFile(f);
    REQUIRE(f.getEntities().size() == 1u);
    auto m = std::dynamic_pointer_cast<MethodEntity>(f.getEntities()[0]);
    REQUIRE(m != nullptr);
    CHECK(m->getParams().empty());
}

TEST_CASE("Logger update forwards to info output") {
    std::stringstream buffer;
    auto* old = std::cout.rdbuf(buffer.rdbuf());
    Logger logger;
    logger.update("hello");
    std::cout.rdbuf(old);
    CHECK(buffer.str().find("hello") != std::string::npos);
}
