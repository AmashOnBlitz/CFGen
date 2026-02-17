#!/usr/bin/env bash
set -euo pipefail

# Colors
CYAN='\033[96m'
GREEN='\033[92m'
YELLOW='\033[93m'
RED='\033[91m'
BLUE='\033[94m'
GRAY='\033[90m'
RESET='\033[0m'
BOLD='\033[1m'

# Configuration
OUTPUT_DIR=build
OBJ_DIR=$OUTPUT_DIR/obj
EXE=$OUTPUT_DIR/cfgen

# Source files
SOURCES=(
    src/main.cpp
    src/globals.cpp
    src/generator.cpp
    src/MacroInterface.cpp
    src/TemplateInterface.cpp
    src/DB.cpp
)

# Compiler flags
CXXFLAGS="-std=c++20 -O2 -g -fno-omit-frame-pointer -pipe -Iinclude"

# Create output directories
mkdir -p "$OUTPUT_DIR" "$OBJ_DIR"

# Track compilation status
COMPILE_NEEDED=0
JOB_COUNT=0
declare -a PIDS=()
declare -a FAILED_SRCS=()

echo
echo -e "${CYAN}============================================================${RESET}"
echo -e "  ${BOLD}Starting Build${RESET}"
echo -e "${CYAN}============================================================${RESET}"
echo
echo -e "${BLUE}[*]${RESET} Analyzing source files..."
echo

# Compile each source file in parallel
for SRC in "${SOURCES[@]}"; do
    BASENAME=$(basename "$SRC" .cpp)
    OBJ="$OBJ_DIR/$BASENAME.o"

    NEEDS_COMPILE=0
    if [ ! -f "$OBJ" ]; then
        NEEDS_COMPILE=1
        echo -e "${YELLOW}    [BUILD]${RESET} $SRC ${GRAY}(new)${RESET}"
    elif [ "$SRC" -nt "$OBJ" ]; then
        NEEDS_COMPILE=1
        echo -e "${YELLOW}    [BUILD]${RESET} $SRC ${GRAY}(modified)${RESET}"
    else
        echo -e "${GRAY}    [CACHE]${RESET} $SRC"
    fi

    if [ "$NEEDS_COMPILE" -eq 1 ]; then
        COMPILE_NEEDED=1
        JOB_COUNT=$((JOB_COUNT + 1))
        # Compile in background, capture errors to a temp file
        ERR_FILE=$(mktemp /tmp/cfgen_build.XXXXXX)
        g++ $CXXFLAGS -c "$SRC" -o "$OBJ" 2>"$ERR_FILE" &
        PIDS+=("$! $SRC $ERR_FILE")
    fi
done

# Wait for all background compilation jobs
if [ "$JOB_COUNT" -gt 0 ]; then
    echo
    echo -e "${BLUE}[*]${RESET} Compiling $JOB_COUNT file(s) in parallel..."

    BUILD_FAILED=0
    for ENTRY in "${PIDS[@]}"; do
        PID=$(echo "$ENTRY" | cut -d' ' -f1)
        SRC=$(echo "$ENTRY" | cut -d' ' -f2)
        ERR_FILE=$(echo "$ENTRY" | cut -d' ' -f3)

        if ! wait "$PID"; then
            echo -e "\n${RED}[ERROR]${RESET} Compilation failed for $SRC"
            if [ -s "$ERR_FILE" ]; then
                cat "$ERR_FILE"
            fi
            BUILD_FAILED=1
        fi
        rm -f "$ERR_FILE"
    done

    if [ "$BUILD_FAILED" -eq 1 ]; then
        echo
        echo -e "${RED}============================================================${RESET}"
        echo -e "  ${BOLD}${RED}BUILD FAILED${RESET}"
        echo -e "${RED}============================================================${RESET}"
        echo
        exit 1
    fi
fi

# Link
if [ "$COMPILE_NEEDED" -eq 1 ] || [ ! -f "$EXE" ]; then
    echo
    echo -e "${CYAN}------------------------------------------------------------${RESET}"
    echo -e "  ${BOLD}Linking${RESET}"
    echo -e "${CYAN}------------------------------------------------------------${RESET}"
    echo
    echo -e "${BLUE}[*]${RESET} Creating executable..."

    if ! g++ "$OBJ_DIR"/*.o -o "$EXE"; then
        echo -e "${RED}[ERROR]${RESET} Linker failed"
        exit 1
    fi

    echo
    echo -e "${GREEN}============================================================${RESET}"
    echo -e "  ${BOLD}${GREEN}BUILD SUCCESSFUL${RESET}"
    echo -e "${GREEN}============================================================${RESET}"
    echo
    echo -e "${GREEN}[OK]${RESET} Output: ${BOLD}$EXE${RESET}"
    echo
else
    echo
    echo -e "${GREEN}[OK]${RESET} Build up to date - nothing to do"
    echo
fi

# Uncomment to run after building
# "$EXE" "$@"