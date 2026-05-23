# DocGen Lite

DocGen Lite — консольная утилита для генерации HTML-документации по C/C++ коду из комментариев в стиле Doxygen.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

## Запуск (длинная команда)

```bash
.\build\docgen.exe -i . -o .\build\docs --exclude .git,build,out --open
```

Параметры:

- `-i`, `--input` — входная директория для анализа
- `-o`, `--output` — выходная директория для документации
- `--exclude dir1,dir2` — исключаемые директории (например: `.git,build,node_modules`)
- `--open` — автоматически открыть `index.html` в браузере после генерации

## Запуск (короткая команда)

В корне проекта выполните:

```bash
.\generate-docs.bat
```

Скрипт автоматически:

- анализирует весь проект (вход `.`)
- записывает документацию в `build/docs`
- исключает `.git`, `build`, `out`
- открывает `build/docs/index.html` в браузере

## Важно про одну папку документации

Рекомендуемый выход всегда один: `build/docs`.  
Перед генерацией старые служебные файлы документации очищаются, поэтому после каждого запуска остается только актуальная версия.

## Синтаксис комментариев

Используйте `///` или `/** ... */` перед классом, функцией или полем.

Пример:

```cpp
/**
 * @brief Разбирает один исходный файл и извлекает сущности.
 * @param file Модель исходного файла для разбора.
 * @return true, если разбор завершился без критических ошибок.
 * @author Команда PKS
 * @version 1.1
 */
bool parseFile(SourceFile& file);
```

Поддерживаемые теги:

- `@brief`
- `@param`
- `@return`
- `@author`
- `@version`
- `@note`
- `@see`

## Тестирование (Catch2)

Unit-тесты и сценарии находятся в каталоге `tests/`.

Сборка с тестами:

```bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
```

Запуск unit-тестов:

```bash
ctest --test-dir build --output-on-failure
```

Запуск сценариев:

```bash
.\build\tests\scenario_cli_run.exe
.\build\tests\scenario_parse_sample.exe
.\build\tests\scenario_generate_html.exe
```

Дополнительные документы по тестированию:

- `tests/SCENARIOS.md` — подробные сценарии для отчета
- `tests/COVERAGE_MATRIX.md` — матрица покрытия тестами

## Контейнеризация (Docker)

### 1) Сборка образа проекта

```bash
docker build -t docgen:latest .
```

### 2) Запуск тестов Catch2 внутри контейнера

Есть отдельный тестовый stage `tester`, в котором выполняется `ctest`.

```bash
docker build --target tester -t docgen:test .
```

Если сборка stage `tester` завершилась успешно, значит тесты в чистом Linux-окружении прошли.

Чтобы явно увидеть вывод тестов:

```bash
docker build --no-cache --progress=plain --target tester -t docgen:test .
```

Или запустить `ctest` напрямую в собранном тестовом образе:

```bash
docker run --rm --entrypoint ctest docgen:test --test-dir build --output-on-failure -V
```

### 3) Запуск основной функции контейнера с аргументами

Контейнер принимает те же аргументы, что и `docgen`.

Пример (Linux/macOS shell):

```bash
docker run --rm -v "$(pwd):/workspace" docgen:latest \
  -i /workspace/src \
  -o /workspace/build/docs \
  --exclude .git,build,out
```

Пример (PowerShell):

```powershell
docker run --rm -v "${PWD}:/workspace" docgen:latest `
  -i /workspace/src `
  -o /workspace/build/docs `
  --exclude .git,build,out
```

### 4) Демонстрация работающего контейнера

Минимальный демонстрационный сценарий:

1. `docker build --target tester -t docgen:test .`
2. `docker build -t docgen:latest .`
3. `docker run --rm -v "${PWD}:/workspace" docgen:latest -i /workspace/src -o /workspace/build/docs --exclude .git,build,out`
4. Проверить, что `build/docs/index.html` создан.

### Примечания по реализации

- Используется многостадийная сборка: `builder`, `tester`, `runtime`.
- В итоговый runtime-образ не попадают временные файлы сборки.
- Все зависимости явно установлены в `Dockerfile`.
