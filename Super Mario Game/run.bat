@echo off
::rmdir /s /q build
cmake -G "MinGW Makefiles" -S . -B VSCode/build
cd VSCode/build
cmake --build .
if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting...
    exit /b %ERRORLEVEL%
)
echo Build successful. Running main.exe...
main.exe
