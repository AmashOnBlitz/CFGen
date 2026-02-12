@echo off
setlocal enabledelayedexpansion

:: Enable ANSI color support
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do set "ESC=%%b"

:: Colors
set "CYAN=%ESC%[96m"
set "GREEN=%ESC%[92m"
set "YELLOW=%ESC%[93m"
set "RED=%ESC%[91m"
set "BLUE=%ESC%[94m"
set "GRAY=%ESC%[90m"
set "RESET=%ESC%[0m"
set "BOLD=%ESC%[1m"

:: Configuration
set OUTPUT_DIR=build
set OBJ_DIR=%OUTPUT_DIR%\obj
set EXE=%OUTPUT_DIR%\cfgen.exe

:: Source files
set SOURCES=src\main.cpp src\strings.c src\globals.cpp src\TemplateInterface.cpp src\TemplateDb.cpp

:: Compiler flags
set CXXFLAGS=-std=c++20 -O0 -g -pipe -Iinclude

:: Create output directories
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%"

:: Track compilation status
set COMPILE_NEEDED=0
set JOB_COUNT=0

echo.
echo %CYAN%============================================================%RESET%
echo   %BOLD%Starting Build%RESET%
echo %CYAN%============================================================%RESET%
echo.
echo %BLUE%[*]%RESET% Analyzing source files...
echo.

:: Compile each source file to object file (parallel)
for %%f in (%SOURCES%) do (
    set "SRC=%%f"
    set "OBJ=%OBJ_DIR%\%%~nf.o"
    set "DONE=%OBJ_DIR%\%%~nf.done"
    
    :: Delete old done marker
    if exist "!DONE!" del "!DONE!"
    
    :: Check if recompilation is needed
    set "NEEDS_COMPILE=0"
    if not exist "!OBJ!" (
        set "NEEDS_COMPILE=1"
        echo %YELLOW%    [BUILD]%RESET% %%f %GRAY%^(new^)%RESET%
    ) else (
        xcopy "%%f" "!OBJ!" /L /D /Y 2>nul | find "1 File(s)" >nul
        if not errorlevel 1 (
            set "NEEDS_COMPILE=1"
            echo %YELLOW%    [BUILD]%RESET% %%f %GRAY%^(modified^)%RESET%
        ) else (
            echo %GRAY%    [CACHE]%RESET% %%f
        )
    )
    
    if !NEEDS_COMPILE! equ 1 (
        set COMPILE_NEEDED=1
        set /a JOB_COUNT+=1
        start /MIN /B cmd /c "g++ %CXXFLAGS% -c %%f -o !OBJ! 2>&1 & echo Done > !DONE!"
    )
)

:: Wait for all compilation jobs to complete
if %JOB_COUNT% gtr 0 (
    echo.
    echo %BLUE%[*]%RESET% Compiling %JOB_COUNT% file^(s^) in parallel...
    
    :wait_loop
    set DONE_COUNT=0
    for %%f in (%SOURCES%) do (
        if exist "%OBJ_DIR%\%%~nf.done" set /a DONE_COUNT+=1
    )
    
    if !DONE_COUNT! lss %JOB_COUNT% (
        timeout /t 1 /nobreak >nul
        goto wait_loop
    )
)

:: Check for compilation errors
set BUILD_FAILED=0
for %%f in (%SOURCES%) do (
    set "OBJ=%OBJ_DIR%\%%~nf.o"
    if not exist "!OBJ!" (
        echo.
        echo %RED%[ERROR]%RESET% Compilation failed for %%f
        set BUILD_FAILED=1
    )
)

if %BUILD_FAILED% equ 1 (
    echo.
    echo %RED%============================================================%RESET%
    echo   %BOLD%%RED%BUILD FAILED%RESET%
    echo %RED%============================================================%RESET%
    echo.
    exit /b 1
)

:: Link object files if needed
if %COMPILE_NEEDED% equ 1 (
    echo.
    echo %CYAN%------------------------------------------------------------%RESET%
    echo   %BOLD%Linking%RESET%
    echo %CYAN%------------------------------------------------------------%RESET%
    echo.
    echo %BLUE%[*]%RESET% Creating executable...
    g++ %OBJ_DIR%\*.o -o %EXE%
    if errorlevel 1 (
        echo %RED%[ERROR]%RESET% Linker failed
        exit /b 1
    )
    echo.
    echo %GREEN%============================================================%RESET%
    echo   %BOLD%%GREEN%BUILD SUCCESSFUL%RESET%
    echo %GREEN%============================================================%RESET%
    echo.
    echo %GREEN%[OK]%RESET% Output: %BOLD%%EXE%%RESET%
    echo.
) else (
    if not exist "%EXE%" (
        echo.
        echo %CYAN%------------------------------------------------------------%RESET%
        echo   %BOLD%Linking%RESET%
        echo %CYAN%------------------------------------------------------------%RESET%
        echo.
        echo %BLUE%[*]%RESET% Creating executable...
        g++ %OBJ_DIR%\*.o -o %EXE%
        if errorlevel 1 (
            echo %RED%[ERROR]%RESET% Linker failed
            exit /b 1
        )
        echo.
        echo %GREEN%[OK]%RESET% Output: %BOLD%%EXE%%RESET%
        echo.
    ) else (
        echo.
        echo %GREEN%[OK]%RESET% Build up to date - nothing to do
        echo.
    )
)

:: Uncomment to run after building
:: %EXE% %*