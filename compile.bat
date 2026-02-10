@echo off

g++ main.cpp strings.c globals.cpp -o main.exe
if errorlevel 1 exit /b 1

main.exe %*
