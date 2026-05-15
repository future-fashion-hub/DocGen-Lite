#include <catch2/catch.hpp>

#include "Config.h"
#include "FileScanner.h"
#include "FileUtils.h"
#include "Logger.h"
#include "test_helpers.h"

TEST_CASE("FileUtils joinPath handles separators") {
    CHECK(fileutils::joinPath("a", "b") == "a/b");
    CHECK(fileutils::joinPath("a/", "b") == "a/b");
    CHECK(fileutils::joinPath("a", "/b") == "a/b");
}

TEST_CASE("FileUtils creates and checks directory") {
    std::string dir = testTempDir("fu_dir");
    CHECK(fileutils::directoryExists(dir));
}

TEST_CASE("FileUtils lists files recursively") {
    std::string dir = testTempDir("fu_list");
    std::string sub = fileutils::joinPath(dir, "sub");
    fileutils::createDirectories(sub);
    writeTextFile(fileutils::joinPath(dir, "a.cpp"), "int a;");
    writeTextFile(fileutils::joinPath(sub, "b.h"), "int b;");
    CHECK(fileutils::listFilesRecursive(dir).size() >= 2u);
}

TEST_CASE("FileUtils removes directory recursively") {
    std::string dir = testTempDir("fu_rm");
    std::string sub = fileutils::joinPath(dir, "x");
    fileutils::createDirectories(sub);
    writeTextFile(fileutils::joinPath(sub, "c.cpp"), "int c;");
    REQUIRE(fileutils::removePathRecursive(dir));
    CHECK_FALSE(fileutils::directoryExists(dir));
}

TEST_CASE("FileScanner reads only supported files") {
    std::string in = testTempDir("scanner_in1");
    std::string out = testTempDir("scanner_out1");
    writeTextFile(fileutils::joinPath(in, "a.cpp"), "int a;");
    writeTextFile(fileutils::joinPath(in, "b.hpp"), "int b;");
    writeTextFile(fileutils::joinPath(in, "c.txt"), "skip");

    Config config;
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out;
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0]};
    REQUIRE(config.loadFromArgs(5, argv));

    Logger logger;
    FileScanner scanner(config, logger);
    CHECK(scanner.scanDirectory().size() == 2u);
}

TEST_CASE("FileScanner reads file contents") {
    std::string in = testTempDir("scanner_in2");
    std::string out = testTempDir("scanner_out2");
    writeTextFile(fileutils::joinPath(in, "a.cpp"), "int f(){return 1;}");

    Config config;
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out;
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0]};
    REQUIRE(config.loadFromArgs(5, argv));

    Logger logger;
    FileScanner scanner(config, logger);
    const auto files = scanner.scanDirectory();
    REQUIRE(files.size() == 1u);
    CHECK(files[0].getContent().find("return 1") != std::string::npos);
}

TEST_CASE("FileScanner applies exclude list") {
    std::string in = testTempDir("scanner_in3");
    std::string out = testTempDir("scanner_out3");
    std::string skip = fileutils::joinPath(in, "skipdir");
    fileutils::createDirectories(skip);
    writeTextFile(fileutils::joinPath(skip, "a.cpp"), "int a;");
    writeTextFile(fileutils::joinPath(in, "b.cpp"), "int b;");

    Config config;
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out, s5 = "--exclude", s6 = "skipdir";
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0]};
    REQUIRE(config.loadFromArgs(7, argv));

    Logger logger;
    FileScanner scanner(config, logger);
    const auto files = scanner.scanDirectory();
    REQUIRE(files.size() == 1u);
    CHECK(files[0].getPath().find("b.cpp") != std::string::npos);
}
