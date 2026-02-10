@echo off

g++ main.cpp strings.c globals.cpp -o cfgen.exe
if errorlevel 1 exit /b 1

cfgen.exe %*
