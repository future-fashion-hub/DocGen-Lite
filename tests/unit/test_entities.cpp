#include <catch2/catch.hpp>

#include <memory>

#include "entities/ClassEntity.h"
#include "entities/Comment.h"
#include "entities/FieldEntity.h"
#include "entities/MethodEntity.h"
#include "entities/Parameter.h"

TEST_CASE("Comment parses brief, details and tags") {
    Comment c;
    c.parse("/// Brief line\n/// detail one\n/// @param x value\n/// @return ok\n");
    CHECK(c.getBrief() == "Brief line");
    CHECK(c.getDetailed() == "detail one");
    REQUIRE(c.getTags().count("@param") == 1u);
    CHECK(c.getTags().at("@return")[0] == "ok");
}

TEST_CASE("Comment parses block comment tags") {
    Comment c;
    c.parse("/**\n * @brief Test\n * @note note text\n */");
    CHECK(c.getTags().at("@note")[0] == "note text");
}

TEST_CASE("Parameter stores name and type") {
    Parameter p("x", "int");
    CHECK(p.getName() == "x");
    CHECK(p.getType() == "int");
}

TEST_CASE("FieldEntity exposes properties") {
    FieldEntity f("count", 10, "int", true);
    CHECK(f.getFieldType() == "int");
    CHECK(f.isConstField());
    CHECK(f.kind() == "field");
}

TEST_CASE("FieldEntity setType affects render") {
    FieldEntity f("count", 10, "int", false);
    f.setType("long");
    CHECK(f.renderHtml().find("long") != std::string::npos);
}

TEST_CASE("MethodEntity builds signature") {
    MethodEntity m("sum", 2, "int", false);
    m.addParam(Parameter("a", "int"));
    m.addParam(Parameter("b", "int"));
    CHECK(m.getSignature() == "int sum(int a, int b)");
}

TEST_CASE("MethodEntity supports static methods") {
    MethodEntity m("create", 2, "Obj*", true);
    CHECK(m.kind() == "method");
    CHECK(m.isStaticMethod());
    CHECK(m.getSignature().find("static") != std::string::npos);
}

TEST_CASE("ClassEntity stores methods and fields") {
    ClassEntity c("A", 1, "Base");
    auto method = std::make_shared<MethodEntity>("m", 2, "void", false);
    auto field = std::make_shared<FieldEntity>("f", 3, "int", false);
    c.addMethod(method);
    c.addField(field);
    CHECK(c.kind() == "class");
    CHECK(c.getParentClass() == "Base");
    REQUIRE(c.getMethods().size() == 1u);
    REQUIRE(c.getFields().size() == 1u);
}

TEST_CASE("ClassEntity render contains children") {
    ClassEntity c("A", 1);
    c.addMethod(std::make_shared<MethodEntity>("m", 2, "void", false));
    c.addField(std::make_shared<FieldEntity>("f", 3, "int", false));
    const std::string html = c.renderHtml();
    CHECK(html.find("void m()") != std::string::npos);
    CHECK(html.find("int") != std::string::npos);
}
