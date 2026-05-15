#include <catch2/catch.hpp>

#include "Parser.h"
#include "SourceFile.h"
#include "strategies/StandardCppStrategy.h"

TEST_CASE("StandardCppStrategy parses class with parent") {
    StandardCppStrategy s;
    ClassMatch out;
    REQUIRE(s.parseClass("class Child : public Base {", out));
    CHECK(out.name == "Child");
    CHECK(out.parent == "Base");
}

TEST_CASE("StandardCppStrategy parses method") {
    StandardCppStrategy s;
    MethodMatch out{};
    REQUIRE(s.parseMethod("static int sum(int a, int b);", out));
    CHECK(out.isStatic);
    CHECK(out.name == "sum");
    CHECK(out.params == "int a, int b");
}

TEST_CASE("StandardCppStrategy parses field") {
    StandardCppStrategy s;
    FieldMatch out{};
    REQUIRE(s.parseField("const long value;", out));
    CHECK(out.isConst);
    CHECK(out.type == "long");
    CHECK(out.name == "value");
}

TEST_CASE("StandardCppStrategy rejects forward declaration as class body") {
    StandardCppStrategy s;
    ClassMatch out{};
    CHECK_FALSE(s.parseClass("struct stat;", out));
}

TEST_CASE("Parser finds class method and field") {
    SourceFile file("sample.h",
                    "/// @brief Sample class\n"
                    "class A {\n"
                    "/// @brief count field\n"
                    "int count;\n"
                    "/// @brief add\n"
                    "int add(int a, int b);\n"
                    "};\n");
    Parser parser;
    parser.parseFile(file);
    REQUIRE(file.getEntities().size() >= 3u);
}

TEST_CASE("Parser extracts tags") {
    SourceFile file("s.h",
                    "/// @brief method brief\n"
                    "/// @param x value\n"
                    "/// @return ok\n"
                    "int run(int x);\n");
    Parser parser;
    parser.parseFile(file);
    REQUIRE(file.getEntities().size() == 1u);
    const auto& tags = file.getEntities()[0]->getComment().getTags();
    CHECK(tags.count("@param") == 1u);
    CHECK(tags.count("@return") == 1u);
}

TEST_CASE("Parser supports block comments") {
    SourceFile file("s.h",
                    "/**\n"
                    " * @brief block brief\n"
                    " * @note details\n"
                    " */\n"
                    "int run();\n");
    Parser parser;
    parser.parseFile(file);
    REQUIRE(file.getEntities().size() == 1u);
    CHECK(file.getEntities()[0]->getComment().getTags().count("@note") == 1u);
}
