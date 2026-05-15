#include <catch2/catch.hpp>

#include <memory>
#include <vector>

#include "Observable.h"
#include "Observer.h"
#include "SourceFile.h"
#include "entities/FieldEntity.h"

namespace {
class TestObserver : public Observer {
public:
    void update(const std::string& message) override { messages.push_back(message); }
    std::vector<std::string> messages;
};

class TestObservable : public Observable {
public:
    void emit(const std::string& msg) { notify(msg); }
};
}

TEST_CASE("SourceFile stores path and content") {
    SourceFile f("a.cpp", "int a;");
    CHECK(f.getPath() == "a.cpp");
    CHECK(f.getContent() == "int a;");
}

TEST_CASE("SourceFile setters update values") {
    SourceFile f;
    f.setPath("b.cpp");
    f.setContent("int b;");
    CHECK(f.getPath() == "b.cpp");
    CHECK(f.getContent() == "int b;");
}

TEST_CASE("SourceFile addEntity stores entity") {
    SourceFile f("x.cpp", "");
    f.addEntity(std::make_shared<FieldEntity>("count", 1, "int", false));
    REQUIRE(f.getEntities().size() == 1u);
}

TEST_CASE("SourceFileBuilder builds configured object") {
    SourceFileBuilder builder;
    auto entity = std::make_shared<FieldEntity>("value", 2, "int", false);
    SourceFile file = builder.setPath("p.cpp").setContent("c").addEntity(entity).build();
    CHECK(file.getPath() == "p.cpp");
    CHECK(file.getEntities().size() == 1u);
}

TEST_CASE("Observable notifies subscribed observers") {
    TestObservable observable;
    TestObserver observer;
    observable.addObserver(&observer);
    observable.emit("hello");
    REQUIRE(observer.messages.size() == 1u);
    CHECK(observer.messages[0] == "hello");
}

TEST_CASE("Observable removeObserver stops notifications") {
    TestObservable observable;
    TestObserver observer;
    observable.addObserver(&observer);
    observable.removeObserver(&observer);
    observable.emit("hello");
    CHECK(observer.messages.empty());
}

TEST_CASE("Observable prevents duplicate registration") {
    TestObservable observable;
    TestObserver observer;
    observable.addObserver(&observer);
    observable.addObserver(&observer);
    observable.emit("x");
    CHECK(observer.messages.size() == 1u);
}
