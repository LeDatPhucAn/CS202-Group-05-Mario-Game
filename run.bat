@echo off
cmake -G "Ninja" -S . -B VSCode/build
cd VSCode/build
cmake --build . -- -j0
if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting...
    exit /b %ERRORLEVEL%
)
echo Build successful. Running main.exe...
main.exe
