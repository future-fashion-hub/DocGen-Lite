cmake_minimum_required(VERSION 3.10)
project(DocGenLite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(docgen)

target_include_directories(docgen PRIVATE include)

target_sources(docgen PRIVATE
    src/main.cpp
    src/Config.cpp
    src/FileScanner.cpp
    src/SourceFile.cpp
    src/Parser.cpp
    src/HTMLGenerator.cpp
    src/Logger.cpp
    src/FileUtils.cpp
    src/entities/CodeEntity.cpp
    src/entities/ClassEntity.cpp
    src/entities/MethodEntity.cpp
    src/entities/FieldEntity.cpp
    src/entities/Parameter.cpp
    src/entities/Comment.cpp
    src/strategies/StandardCppStrategy.cpp
)

file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
