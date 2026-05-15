include(FetchContent)

FetchContent_Declare(
    catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.10.zip
)
FetchContent_MakeAvailable(catch2)

add_executable(docgen_unit_tests
    unit/test_main.cpp
    unit/test_config.cpp
    unit/test_fileutils_and_scanner.cpp
    unit/test_entities.cpp
    unit/test_parser_and_strategy.cpp
    unit/test_sourcefile_and_observer.cpp
    unit/test_html_generator.cpp
    unit/test_compliance_extended.cpp
    unit/test_strict_coverage.cpp
)
target_link_libraries(docgen_unit_tests PRIVATE docgen_core Catch2::Catch2)
target_include_directories(docgen_unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(${catch2_SOURCE_DIR}/contrib/Catch.cmake)
catch_discover_tests(docgen_unit_tests)

add_executable(scenario_cli_run scenarios/scenario_cli_run.cpp)
target_link_libraries(scenario_cli_run PRIVATE docgen_core)
target_include_directories(scenario_cli_run PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(scenario_parse_sample scenarios/scenario_parse_sample.cpp)
target_link_libraries(scenario_parse_sample PRIVATE docgen_core)
target_include_directories(scenario_parse_sample PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(scenario_generate_html scenarios/scenario_generate_html.cpp)
target_link_libraries(scenario_generate_html PRIVATE docgen_core)
target_include_directories(scenario_generate_html PRIVATE ${CMAKE_SOURCE_DIR}/include)
