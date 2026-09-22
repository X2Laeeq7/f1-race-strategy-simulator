@echo off
g++ -std=c++17 src\main.cpp -o sim.exe
if %errorlevel% neq 0 (
    echo Build failed.
    pause
    exit /b %errorlevel%
)
echo Build succeeded. 
