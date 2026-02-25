#ifndef VERSION_HEADER
#define VERSION_HEADER
#define VERSION "2.0.0"

#include "globals.h"
#include <fstream>
#include <cstdio>
#include <string>

static std::string FetchRemoteVersion_System()
{
#if defined(_WIN32)

    system(
        "curl -fsSL https://raw.githubusercontent.com/AmashOnBlitz/CFGen/main/docs/version "
        "> cfgen_remote_version.tmp 2>nul"
    );

#elif defined(__APPLE__)

    system(
        "curl -fsSL https://raw.githubusercontent.com/AmashOnBlitz/CFGen/main/docs/version "
        "> cfgen_remote_version.tmp 2>/dev/null"
    );

#elif defined(__linux__)

    system(
        "curl -fsSL https://raw.githubusercontent.com/AmashOnBlitz/CFGen/main/docs/version "
        "> cfgen_remote_version.tmp 2>/dev/null || "
        "wget -q -O cfgen_remote_version.tmp "
        "https://raw.githubusercontent.com/AmashOnBlitz/CFGen/main/docs/version"
    );

#else
    return "";
#endif

    std::ifstream file("cfgen_remote_version.tmp");
    if (!file.is_open())
        return "";

    std::string version;
    std::getline(file, version);
    file.close();

    std::remove("cfgen_remote_version.tmp");
    return version;
}

static int CompareVersions(const std::string& a, const std::string& b)
{
    int aMaj = 0, aMin = 0, aPat = 0;
    int bMaj = 0, bMin = 0, bPat = 0;

    sscanf(a.c_str(), "%d.%d.%d", &aMaj, &aMin, &aPat);
    sscanf(b.c_str(), "%d.%d.%d", &bMaj, &bMin, &bPat);

    if (aMaj != bMaj) return aMaj - bMaj;
    if (aMin != bMin) return aMin - bMin;
    return aPat - bPat;
}

std::string GetVersionInfo()
{
    Constants& C = Constants::Instance();
    std::string out;

    out += C.GetString(STRINGCODE_VERSION_HEADER);
    out += C.GetString(STRINGCODE_VERSION_LOCAL);
    out += VERSION;
    out += "\n";

    out += C.GetString(STRINGCODE_VERSION_CHECKING);

    std::string remote = FetchRemoteVersion_System();

    if (remote.empty()) {
        out += C.GetErrorString(ERRORCODE_FAILED_TO_FETCH_REMOTE_VERSION);
        return out;
    }

    out += C.GetString(STRINGCODE_VERSION_REMOTE);
    out += remote;
    out += "\n";

    int cmp = CompareVersions(VERSION, remote);

    if (cmp == 0) {
        out += C.GetString(STRINGCODE_VERSION_UP_TO_DATE);
    }
    else if (cmp < 0) {
        out += C.GetString(STRINGCODE_VERSION_OUTDATED);
    }
    else {
        out += C.GetString(STRINGCODE_VERSION_BETA);
    }

    return out;
}

#endif // VERSION_HEADER
