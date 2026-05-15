#include <catch2/catch.hpp>

#include <string>

#include "Config.h"
#include "test_helpers.h"

TEST_CASE("Config parses short flags") {
    Config config;
    std::string in = testTempDir("cfg_in1");
    std::string out = testTempDir("cfg_out1");
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out;
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0]};
    REQUIRE(config.loadFromArgs(5, argv));
    CHECK(config.inputPath() == in);
    CHECK(config.outputPath() == out);
}

TEST_CASE("Config parses long flags and open") {
    Config config;
    std::string in = testTempDir("cfg_in2");
    std::string out = testTempDir("cfg_out2");
    char arg0[] = "docgen";
    std::string s1 = "--input", s2 = in, s3 = "--output", s4 = out, s5 = "--open";
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0], &s5[0]};
    REQUIRE(config.loadFromArgs(6, argv));
    CHECK(config.shouldOpenAfterGeneration());
}

TEST_CASE("Config parses exclude list") {
    Config config;
    std::string in = testTempDir("cfg_in3");
    std::string out = testTempDir("cfg_out3");
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out, s5 = "--exclude", s6 = "a,b,c";
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0]};
    REQUIRE(config.loadFromArgs(7, argv));
    REQUIRE(config.excludeDirs().size() == 3u);
    CHECK(config.excludeDirs()[1] == "b");
}

TEST_CASE("Config validate fails on missing input") {
    Config config;
    std::string out = testTempDir("cfg_out4");
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = "missing-dir", s3 = "-o", s4 = out;
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0]};
    REQUIRE(config.loadFromArgs(5, argv));
    std::string error;
    CHECK_FALSE(config.validate(&error));
    CHECK(error.find("Input path does not exist") != std::string::npos);
}

TEST_CASE("Config validate passes for valid input") {
    Config config;
    std::string in = testTempDir("cfg_in5");
    std::string out = testTempDir("cfg_out5");
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out;
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0]};
    REQUIRE(config.loadFromArgs(5, argv));
    std::string error;
    CHECK(config.validate(&error));
}
