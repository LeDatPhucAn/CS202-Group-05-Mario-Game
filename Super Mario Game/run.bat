@echo off
rmdir -Recurse -Force VSCode
cmake -G "Ninja" -S . -B VSCode/build
cd VSCode/build
cmake --build .
if %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting...
    exit
)
echo Build successful. Running main.exe...
main.exe
