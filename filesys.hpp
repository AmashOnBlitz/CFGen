#pragma once 
#include <cstdlib>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

fs::path getAppDataPath()
{
#ifdef _WIN32
    if (const char* appdata = std::getenv("APPDATA"))
        return fs::path(appdata);

#elif __APPLE__
    if (const char* home = std::getenv("HOME"))
        return fs::path(home) / "Library" / "Application Support";

#else // Linux / Unix
    if (const char* xdg = std::getenv("XDG_CONFIG_HOME"))
        return fs::path(xdg);

    if (const char* home = std::getenv("HOME"))
        return fs::path(home) / ".config";
#endif

    throw std::runtime_error("Cannot determine app data directory");
}

fs::path getAppDataDir()
{
    fs::path base = getAppDataPath();
    fs::path appDir = base / "CFGen"; 
    fs::create_directories(appDir); 
    return appDir;
}
