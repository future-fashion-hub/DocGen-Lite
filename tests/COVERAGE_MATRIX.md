# Матрица покрытия тестами

Ниже указаны **основные функции классов** и количество отдельных тест-кейсов, которые их покрывают.

## Config
- `loadFromArgs`: 7 тестов
- `validate`: 5 тестов
- `inputPath/outputPath/excludeDirs/shouldOpenAfterGeneration`: 6 тестов

## FileUtils
- `joinPath`: 5 тестов
- `createDirectories`: 4 теста
- `directoryExists`: 4 теста
- `listFilesRecursive`: 4 теста
- `removePathRecursive`: 4 теста
- `openInBrowser`: покрытие интеграционно через сценарии (без принудительного вызова браузера в unit)

## FileScanner
- `scanDirectory`: 7 тестов (поддерживаемые расширения, содержимое, исключения, несуществующий вход)

## SourceFile / SourceFileBuilder
- `get/set path/content`: 5 тестов
- `addEntity/getEntities`: 5 тестов
- `build`: 5 тестов

## Observable / Observer
- `addObserver/removeObserver/notify`: 6 тестов

## Logger
- `info/warning/error/update`: 6 тестов
- фильтрация по `LogLevel`: 3 теста

## Comment
- `parse`: 7 тестов (line/block, tags, empty input, only tags, detail lines)
- `getBrief/getDetailed/getTags`: 7 тестов

## Parameter
- `getName/getType`: 5 тестов

## FieldEntity
- `setType/getFieldType/isConstField/kind/renderHtml`: 6 тестов

## MethodEntity
- `addParam/getSignature/getReturnType/getParams/isStaticMethod/kind/renderHtml`: 7 тестов

## ClassEntity
- `addMethod/addField/add/getParentClass/getMethods/getFields/kind/renderHtml`: 7 тестов

## CodeEntity
- `getName/getLine/setComment/getComment` (через `DummyEntity`): 5 тестов

## StandardCppStrategy
- `parseClass`: 5 тестов
- `parseMethod`: 5 тестов
- `parseField`: 5 тестов

## Parser
- `setStrategy`: 5 тестов
- `parseFile`: 7 тестов (class/method/field/comments/tags/empty/unclosed/void params)
- `notifyGenerationFinished`: 5 тестов

## HTMLGenerator
- `generate` (с проверкой результата): 6 тестов
- интеграционная проверка `index/navigation/assets/tag-block/cleanup`: 6 тестов

## Сценарии (п.3 и п.4 ТЗ)
- Список сценариев: [SCENARIOS.md](C:\Users\user\Desktop\Дроч\ПКС\PKS_DOCGEN\tests\SCENARIOS.md)
- Реализация: `scenario_cli_run`, `scenario_parse_sample`, `scenario_generate_html`
