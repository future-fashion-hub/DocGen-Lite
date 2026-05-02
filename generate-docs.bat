@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "DOCGEN_EXE=%SCRIPT_DIR%build\docgen.exe"

if not exist "%DOCGEN_EXE%" (
    echo [ERROR] File not found: "%DOCGEN_EXE%"
    echo Build the project first:
    echo   cmake -S . -B build
    echo   cmake --build build
    exit /b 1
)

echo [INFO] Generating documentation...
"%DOCGEN_EXE%" -i "." -o ".\build\docs" --exclude .git,build,out --open
set "EXIT_CODE=%ERRORLEVEL%"

if not "%EXIT_CODE%"=="0" (
    echo [ERROR] Generation failed with code %EXIT_CODE%.
    exit /b %EXIT_CODE%
)

echo [INFO] Done. Documentation: ".\build\docs\index.html"
exit /b 0
