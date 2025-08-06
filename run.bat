@echo off
REM 1. Cấu hình project với Ninja
cmake -G "Ninja" -S . -B VSCode/build
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed. Exiting...
    exit /b %ERRORLEVEL%
)

REM 2. Chuyển vào thư mục build
cd VSCode\build

REM 3. Build song song, số job = số lõi CPU
cmake --build . --parallel %NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed. Exiting...
    exit /b %ERRORLEVEL%
)

REM 4. Nếu build thành công, chạy chương trình
echo Build successful. Running main.exe...
main.exe
