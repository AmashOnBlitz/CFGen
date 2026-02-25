#include "generator.h"
#include "TemplateInterface.h"
#include "globals.h"
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_set>

std::filesystem::path ProcFilePath;

static size_t getLineNumber(const std::string& text, size_t pos);

static std::tm getLocalTime(std::time_t t);

// ============================================================
// Default macros
// ============================================================
bool resolveDefaultMacro(const std::string& name, std::string& out)
{
    // ================= FILE INFO =================
    if (name == "filename") {
        out = ProcFilePath.filename().string();
        return true;
    }

    if (name == "stem") {
        out = ProcFilePath.stem().string();
        return true;
    }

    if (name == "abspath") {
        out = std::filesystem::absolute(ProcFilePath).string();
        return true;
    }

    if (name == "dir") {
        out = ProcFilePath.parent_path().string();
        return true;
    }

    if (name == "cwd") {
        out = std::filesystem::current_path().string();
        return true;
    }

    if (name == "ext") {
        out = ProcFilePath.extension().string();
        return true;
    }

    // ============= TIME RELATED =================
    std::time_t t = std::time(nullptr);
    std::tm tm = getLocalTime(t);

    if (name == "date") {
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm);
        out = buf;
        return true;
    }

    if (name == "time") {
        char buf[32];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
        out = buf;
        return true;
    }

    if (name == "year") {
        out = std::to_string(1900 + tm.tm_year);
        return true;
    }

    if (name == "weekday") {
        char buf[16];
        std::strftime(buf, sizeof(buf), "%A", &tm);
        out = buf;
        return true;
    }

    if (name == "month") {
        char buf[16];
        std::strftime(buf, sizeof(buf), "%B", &tm);
        out = buf;
        return true;
    }

    if (name == "iso_date") {
        char buf[32];
        std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm);
        out = buf;
        return true;
    }

    if (name == "unix_time") {
        out = std::to_string(static_cast<long long>(t));
        return true;
    }

    return false;
}

bool resolveUserMacro(
    const std::string& name,
    const std::vector<std::pair<std::string, std::string>>& macros,
    std::string& out
){
    for (const auto& m : macros) {
        if (m.first == name) {
            out = m.second;
            return true;
        }
    }
    std::cout << Constants::Instance().GetString(STRINGCODE_MACR_NOT_REGISTERED) + name + " | Ignoring Macro\n";
    return false;
}

// ============================================================
// Transform functions
// ============================================================
using TransformFn = std::function<std::string(const std::string&)>;

std::unordered_map<std::string, TransformFn> buildTransforms()
{
    return {
        { "upper", [](const std::string& s) {
            std::string r = s;
            std::transform(r.begin(), r.end(), r.begin(),
                [](unsigned char c){ return std::toupper(c); });
            return r;
        }},
        { "lower", [](const std::string& s) {
            std::string r = s;
            std::transform(r.begin(), r.end(), r.begin(),
                [](unsigned char c){ return std::tolower(c); });
            return r;
        }},
        { "trim", [](const std::string& s) {
            auto start = s.find_first_not_of(" \t\n\r");
            auto end   = s.find_last_not_of(" \t\n\r");
            if (start == std::string::npos) return std::string{};
            return s.substr(start, end - start + 1);
        }},
        { "snake", [](const std::string& s) {
        std::string r;
        for (char c : s) {
            if (std::isupper(c)) {
                if (!r.empty()) r += '_';
                r += std::tolower(c);
            } else if (c == ' ') {
                r += '_';
            } else {
                r += c;
            }
        }
        return r;
    }},
    { "camel", [](const std::string& s) {
        std::string r;
        bool cap = false;
        for (char c : s) {
            if (c == '_' || c == ' ') {
                cap = true;
            } else {
                r += cap ? std::toupper(c) : std::tolower(c);
                cap = false;
            }
        }
        return r;
    }},
    { "pascal", [](const std::string& s) {
        std::string r;
        bool cap = true;
        for (char c : s) {
            if (c == '_' || c == ' ') {
                cap = true;
            } else {
                r += cap ? std::toupper(c) : std::tolower(c);
                cap = false;
            }
        }
        return r;
    }},
    { "title", [](const std::string& s) {
        std::string r;
        bool cap = true;
        for (char c : s) {
            if (std::isspace(c)) {
                cap = true;
                r += c;
            } else {
                r += cap ? std::toupper(c) : std::tolower(c);
                cap = false;
            }
        }
        return r;
    }},
    { "reverse", [](const std::string& s) {
        return std::string(s.rbegin(), s.rend());
    }},
    { "slug", [](const std::string& s) {
        std::string r;
        for (char c : s) {
            if (std::isalnum(c))
                r += std::tolower(c);
            else if (c == ' ' || c == '_' || c == '-')
                r += '-';
        }
        return r;
    }},
    };
}

std::string resolveMacroBody(
    const std::string& body,
    const std::vector<std::pair<std::string, std::string>>& macros,
    const std::unordered_map<std::string, TransformFn>& transforms,
    int depth,
    size_t line,
    const std::string& fullMacro
)
{
        constexpr int MAX_TRANSFORM_DEPTH = 32;

    // Depth check
    if (depth > MAX_TRANSFORM_DEPTH) {
        std::cout
        << Constants::Instance().GetErrorString(ERRORCODE_MACRO_UNMATCHED_BRACES)
        << "\n  at line: " << line
        << "\n  macro  : ${" << fullMacro << "}\n";
        return "";
    }

    // Transform syntax check
    size_t colon = body.find(':');
    if (colon != std::string::npos) {

        std::string algo = body.substr(0, colon);
        std::string inner = body.substr(colon + 1);

        // Must be ${...}
        if (inner.size() < 4 || inner.substr(0, 2) != "${" || inner.back() != '}') {
            std::cout
            << Constants::Instance().GetErrorString(ERRORCODE_INVALID_TRANSFORM_SYNTAX)
            << "\n  at line: " << line
            << "\n  macro  : ${" << fullMacro << "}\n";
            return "";
        }

        auto it = transforms.find(algo);
        if (it == transforms.end())
            return "";

        std::string innerBody = inner.substr(2, inner.size() - 3);

        // Recursive resolution
        std::string resolved = resolveMacroBody(
            innerBody,
            macros,
            transforms,
            depth + 1,
            line,       
            fullMacro    
        );

        // if (resolved.empty())
        //     return "";

        return it->second(resolved);
    }

    // Plain macro
    std::string value;
    if (resolveDefaultMacro(body, value))
        return value;

    if (resolveUserMacro(body, macros, value))
        return value;

    return "";
}
void handleTemplateMacros(
    std::string& content,
    const std::vector<std::pair<std::string, std::string>>& macros
){
    const auto transforms = buildTransforms();
    size_t pos = 0;

    while ((pos = content.find("${", pos)) != std::string::npos) {

        size_t start = pos + 2;
        size_t i = start;
        int braceDepth = 1;
        constexpr int MAX_BRACE_DEPTH = 32;

        while (i < content.size() && braceDepth > 0) {
            if (content[i] == '{') {
                braceDepth++;
                if (braceDepth > MAX_BRACE_DEPTH) {
                    std::cout
                        << Constants::Instance().GetErrorString(ERRORCODE_MACRO_NESTING_TOO_DEEP)
                        << MAX_BRACE_DEPTH << ")\n"
                        << "  at line: " << getLineNumber(content, pos)
                        << "\n  near  : " << content.substr(pos, 40) << "...\n";
                    return;
                }
            }
            else if (content[i] == '}') {
                braceDepth--;
            }
            i++;
        }

        if (braceDepth != 0) {
            std::cout
                << Constants::Instance().GetErrorString(
                    ERRORCODE_MACRO_UNMATCHED_BRACES
                ) << "\n";
            return;
        }

        size_t end = i - 1; // matched '}'

        std::string body = content.substr(start, end - start);
        size_t line = getLineNumber(content, pos);

        std::string replacement =
            resolveMacroBody(
                body,
                macros,
                transforms,
                0,
                line,
                body
            );

        content.replace(pos, (end - pos) + 1, replacement);
        pos += replacement.length();
    }
}

std::vector<std::string> parseRuntimeParams(int argc, const char* argv[])
{
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.size() >= 3 && arg.substr(0, 3) == "-p<" && arg.back() == '>') {
            std::string inner = arg.substr(3, arg.size() - 4);

            std::vector<std::string> out;
            size_t pos = 0;
            while (true) {
                size_t comma = inner.find(',', pos);
                if (comma == std::string::npos) {
                    out.push_back(inner.substr(pos));
                    break;
                }
                out.push_back(inner.substr(pos, comma - pos));
                pos = comma + 1;
            }
            return out;
        }
    }
    return {};
}

static void printRuntimeBindings(
    const std::vector<std::string>& registerOrder,
    const std::vector<std::pair<std::string, std::string>>& macros,
    size_t expectedCount,
    size_t providedCount
) {
    if (registerOrder.empty()) return;

    std::cout << "\nRuntime Variables:\n";
    std::cout << "------------------\n";

    for (const auto& name : registerOrder) {
        auto it = std::find_if(
            macros.begin(), macros.end(),
            [&](const auto& p) { return p.first == name; }
        );

        if (it != macros.end() && !it->second.empty())
            std::cout << name << "\t" << "---> " << it->second << "\n";
        else
            std::cout << name << "\t" << "---> ?\n";
    }

    if (providedCount < expectedCount) {
        std::cout
            << "\n[Error] Template requires "
            << expectedCount
            << " runtime variables, but only "
            << providedCount
            << " were provided.\n";
    }
}

bool processRuntimeRegisters(
    std::string& content,
    const std::vector<std::string>& runtimeParams,
    std::vector<std::pair<std::string, std::string>>& outMacros,
    std::vector<std::string>& registerOrder
) {
    std::string result;
    size_t paramIndex = 0;
    bool missingParam = false;
    std::unordered_set<std::string> seen;

    size_t pos = 0;
    while (pos < content.size()) {
        size_t end = content.find('\n', pos);
        if (end == std::string::npos)
            end = content.size();

        std::string line = content.substr(pos, end - pos);
        std::string trimmed = line;
        trimmed.erase(0, trimmed.find_first_not_of(" \t"));

        if (trimmed.size() >= 9 &&
            std::equal(trimmed.begin(), trimmed.begin() + 9,
                "@register", [](char a, char b) {
                    return std::tolower(a) == std::tolower(b);
                }))
        {
            std::string var = trimmed.substr(9);
            var.erase(0, var.find_first_not_of(" \t"));

            if (var.empty()) {
                std::cout
                    << Constants::Instance().GetErrorString(
                        ERRORCODE_EMPTY_REGISTER_NAME)
                    << "\n";
                return false;
            }

            if (!seen.insert(var).second) {
                std::cout
                    << Constants::Instance().GetErrorString(
                        ERRORCODE_DUPLICATE_REGISTER)
                    << var << "\n";
                return false;
            }

            registerOrder.push_back(var);

            if (paramIndex < runtimeParams.size()) {
                outMacros.emplace_back(var, runtimeParams[paramIndex]);
            } else {
                outMacros.emplace_back(var, "");
                missingParam = true;
            }

            paramIndex++;
        }
        else {
            result += line;
            if (end < content.size()) result += '\n';
        }

        pos = end + 1;
    }

    content.swap(result);

    if (!registerOrder.empty()) {
        printRuntimeBindings(
            registerOrder,
            outMacros,
            registerOrder.size(),
            runtimeParams.size()
        );
    }

    if (missingParam) {
        return false;
    }

    if (paramIndex < runtimeParams.size()) {
        std::cout
            << Constants::Instance().GetErrorString(
                ERRORCODE_EXTRA_RUNTIME_PARAMS)
            << "\n";
    }

    return true;
}


std::string GenFile(int argc, const char* argv[])
{
    std::string outname(argv[2]);
    std::string templateName(argv[3]);
    ProcFilePath = std::filesystem::path(outname);

    if (!queryTemplateExists(templateName)) {
        return Constants::Instance().GetErrorString(
            ERRORCODE_TEMPLATE_NOT_FOUND
        );
    }

    bool ok = false;
    std::string templateContent = getTemplateScript(templateName, ok);
    if (!ok)
        return templateContent;

    auto runtimeParams = parseRuntimeParams(argc, argv);
    std::vector<std::pair<std::string, std::string>> runtimeMacros;
    std::vector<std::string> registerOrder;

    if (!processRuntimeRegisters(
            templateContent,
            runtimeParams,
            runtimeMacros,
            registerOrder
        ))
    {
        return Constants::Instance().GetErrorString(
            ERRORCODE_RUNTIME_PARAM_MISSING
        );
    }

    auto macros = getMacroTable();
    macros.insert(macros.end(),
              runtimeMacros.begin(),
              runtimeMacros.end());
    handleTemplateMacros(templateContent, macros);

    if (!std::filesystem::exists(ProcFilePath.parent_path()) && ProcFilePath.has_parent_path()){
        std::filesystem::create_directory(ProcFilePath.parent_path());
    }

    if (std::filesystem::exists(ProcFilePath)){
        return Constants::Instance().GetErrorString(ERRORCODE_FILE_ALR_EXISTS);
    }

    std::ofstream out(outname);
    if (!out.is_open()) {
        return Constants::Instance().GetErrorString(ERRORCODE_FAILED_TO_GEN_FILE) + outname;
    }

    out << templateContent;
    out.close();

    return Constants::Instance().GetString(STRINGCODE_FILE_GENERATED) + outname;
}


static size_t getLineNumber(const std::string& text, size_t pos)
{
    return 1 + std::count(text.begin(), text.begin() + pos, '\n');
}

static std::tm getLocalTime(std::time_t t)
{
    std::tm tm{};

#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    return tm;
}
