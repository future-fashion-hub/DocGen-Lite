#include <catch2/catch.hpp>

#include <memory>
#include <type_traits>

#include "Config.h"
#include "FileScanner.h"
#include "FileUtils.h"
#include "Logger.h"
#include "Observer.h"
#include "Parser.h"
#include "SourceFile.h"
#include "entities/ClassEntity.h"
#include "entities/CodeEntity.h"
#include "entities/Comment.h"
#include "entities/FieldEntity.h"
#include "entities/MethodEntity.h"
#include "entities/Parameter.h"
#include "strategies/ParsingStrategy.h"
#include "test_helpers.h"

namespace {
class DummyEntity : public CodeEntity {
public:
    DummyEntity(const std::string& n, std::size_t l) : CodeEntity(n, l) {}
    std::string kind() const override { return "dummy"; }
    std::string renderHtml() const override { return "<x/>"; }
};

class CountingObserver : public Observer {
public:
    void update(const std::string& message) override {
        ++count;
        last = message;
    }
    int count = 0;
    std::string last;
};

class StubStrategy : public ParsingStrategy {
public:
    bool parseClass(const std::string& line, ClassMatch& out) const override {
        if (line == "stubclass") {
            out.name = "StubClass";
            out.parent = "Base";
            return true;
        }
        return false;
    }
    bool parseMethod(const std::string& line, MethodMatch& out) const override {
        if (line == "stubmethod") {
            out.returnType = "int";
            out.name = "stub";
            out.params = "int x";
            out.isStatic = false;
            return true;
        }
        return false;
    }
    bool parseField(const std::string& line, FieldMatch& out) const override {
        if (line == "stubfield") {
            out.type = "long";
            out.name = "v";
            out.isConst = true;
            return true;
        }
        return false;
    }
};
}

TEST_CASE("Config default exclude list has base directories") {
    Config c;
    const auto& ex = c.excludeDirs();
    REQUIRE(ex.size() >= 3u);
    CHECK(ex[0] == ".git");
    CHECK(ex[1] == "build");
    CHECK(ex[2] == "out");
}

TEST_CASE("Config validate fails when args are incomplete") {
    Config c;
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = ".";
    char* argv[] = {arg0, &s1[0], &s2[0]};
    CHECK_FALSE(c.loadFromArgs(3, argv));
}

TEST_CASE("Config parse keeps exclude empty entries removed") {
    Config c;
    std::string in = testTempDir("cfg_ext_in");
    std::string out = testTempDir("cfg_ext_out");
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = in, s3 = "-o", s4 = out, s5 = "--exclude", s6 = "a,,b,";
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0], &s5[0], &s6[0]};
    REQUIRE(c.loadFromArgs(7, argv));
    REQUIRE(c.excludeDirs().size() == 2u);
    CHECK(c.excludeDirs()[0] == "a");
    CHECK(c.excludeDirs()[1] == "b");
}

TEST_CASE("FileUtils joinPath handles empty sides") {
    CHECK(fileutils::joinPath("", "x") == "x");
    CHECK(fileutils::joinPath("x", "") == "x");
}

TEST_CASE("FileUtils removePathRecursive returns true for missing path") {
    CHECK(fileutils::removePathRecursive("tmp_tests/not_existing_path") == true);
}

TEST_CASE("FileUtils listFilesRecursive returns empty for missing root") {
    CHECK(fileutils::listFilesRecursive("tmp_tests/missing_root").empty());
}

TEST_CASE("FileScanner returns empty if input dir missing") {
    Config c;
    std::string out = testTempDir("scanner_miss_out");
    char arg0[] = "docgen";
    std::string s1 = "-i", s2 = "tmp_tests/no_such_input", s3 = "-o", s4 = out;
    char* argv[] = {arg0, &s1[0], &s2[0], &s3[0], &s4[0]};
    REQUIRE(c.loadFromArgs(5, argv));
    Logger logger;
    FileScanner scanner(c, logger);
    CHECK(scanner.scanDirectory().empty());
}

TEST_CASE("CodeEntity exposes core properties") {
    DummyEntity d("entity", 42);
    CHECK(d.getName() == "entity");
    CHECK(d.getLine() == 42u);
    CHECK(d.kind() == "dummy");
    CHECK(d.renderHtml() == "<x/>");
}

TEST_CASE("CodeEntity stores comment through setComment") {
    DummyEntity d("entity", 1);
    Comment c;
    c.parse("/// @brief brief");
    d.setComment(c);
    CHECK(d.getComment().getTags().count("@brief") == 1u);
    CHECK(d.getComment().getTags().at("@brief")[0] == "brief");
}

TEST_CASE("Comment parses multiline details") {
    Comment c;
    c.parse("/// brief\n/// detail1\n/// detail2\n");
    CHECK(c.getBrief() == "brief");
    CHECK(c.getDetailed() == "detail1\ndetail2");
}

TEST_CASE("MethodEntity getParams reflects added parameters") {
    MethodEntity m("f", 1, "void", false);
    m.addParam(Parameter("a", "int"));
    m.addParam(Parameter("b", "double"));
    REQUIRE(m.getParams().size() == 2u);
    CHECK(m.getParams()[0].getName() == "a");
    CHECK(m.getParams()[1].getType() == "double");
}

TEST_CASE("ClassEntity add also accepts generic child") {
    ClassEntity cls("C", 1);
    auto child = std::make_shared<DummyEntity>("x", 2);
    cls.add(child);
    CHECK(cls.kind() == "class");
}

TEST_CASE("Observer and ParsingStrategy are polymorphic") {
    CHECK(std::is_polymorphic<Observer>::value);
    CHECK(std::is_polymorphic<ParsingStrategy>::value);
}

TEST_CASE("Parser notifies observers during parse") {
    Parser parser;
    CountingObserver obs;
    parser.addObserver(&obs);
    SourceFile file("f.h", "int a();\n");
    parser.parseFile(file);
    CHECK(obs.count >= 2);
    CHECK(obs.last.find("Finished parsing file") != std::string::npos);
}

TEST_CASE("Parser supports strategy override") {
    Parser parser;
    parser.setStrategy(std::unique_ptr<ParsingStrategy>(new StubStrategy()));
    SourceFile file("stub.h", "stubclass\nstubmethod\nstubfield\n");
    parser.parseFile(file);
    REQUIRE(file.getEntities().size() == 3u);
    CHECK(file.getEntities()[0]->getName() == "StubClass");
    CHECK(file.getEntities()[1]->getName() == "stub");
    CHECK(file.getEntities()[2]->getName() == "v");
}

TEST_CASE("Parser notifyGenerationFinished triggers observer") {
    Parser parser;
    CountingObserver obs;
    parser.addObserver(&obs);
    parser.notifyGenerationFinished();
    REQUIRE(obs.count == 1);
    CHECK(obs.last == "Documentation generation finished");
}
