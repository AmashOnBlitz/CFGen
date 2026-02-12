#include "TemplateDb.h"
#include "filesys.hpp"
#include "globals.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

#define TMP_FILE_NAME "templates.map"
#define TMP_VAL_DELIM ".key<=@=>val."

std::vector<std::pair<std::string,std::string>> templateTable;
bool templateExists(const std::string& name);
void loadTemplates();

std::string addTemplate(std::string name, std::string value)
{
    fs::path tmpfile = getAppDataDir() / TMP_FILE_NAME;
    if (!fs::exists(tmpfile.parent_path()) && tmpfile.has_parent_path()){
        if(!fs::create_directories(tmpfile.parent_path())){
            return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE);
        }
    }

    loadTemplates();
    if(templateExists(name)){
        return Constants::Instance().GetErrorString(ERRORCODE_TEMPLATE_ALR_EXISTS);
    }

    fs::path inScript = value;
    if (!fs::exists(inScript) || !fs::is_regular_file(inScript)) {
        return Constants::Instance().GetErrorString(
            ERRORCODE_CANNOT_OPEN_PROVIDED_FILE
        );
    }

    fs::path outbase = getAppDataDir() / name;
    if (!fs::exists(outbase)){
        if(!fs::create_directory(outbase)){
            return Constants::Instance().GetErrorString(
            ERRORCODE_CANNOT_COPY_SCRIPT_FILE
            );
        }
    }
    fs::path outScript = outbase / inScript.filename();

    try {
    fs::copy_file(
        inScript,
        outScript,
        fs::copy_options::overwrite_existing
    );
    }
    catch (const fs::filesystem_error&) {
        return Constants::Instance().GetErrorString(
            ERRORCODE_CANNOT_COPY_SCRIPT_FILE
        );
    }
    
    std::fstream file;
    file.open(tmpfile, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE);
    }

    file << name << TMP_VAL_DELIM << inScript.filename().string() << "\n";
    file.close();

    return "";
}

std::string removeTemplate(const std::string& name)
{
    loadTemplates();
    
    if (!templateExists(name)) {
        return Constants::Instance().GetErrorString(ERRORCODE_TEMPLATE_NOT_FOUND_FOR_DEL);
    }

    std::string scriptFilename;
    for (auto it = templateTable.begin(); it != templateTable.end(); ++it) {
        if (it->first == name) {
            scriptFilename = it->second;
            templateTable.erase(it);
            break;
        }
    }

    fs::path tmpfile = getAppDataDir() / TMP_FILE_NAME;
    std::ofstream file(tmpfile);
    if (!file.is_open()) {
        return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_WRITE_TEMPLATE_FILE);
    }

    for (const auto& pair : templateTable) {
        file << pair.first << TMP_VAL_DELIM << pair.second << "\n";
    }
    file.close(); 

    fs::path scriptPathBase = getAppDataDir() / name;
    fs::path scriptPath = scriptPathBase / fs::path(scriptFilename).filename();
    if (fs::exists(scriptPath)) {
        std::error_code ec;
        fs::remove(scriptPath, ec);
        if (ec) {
            return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_DELETE_SCRIPT_FILE);
        }
    }

    return "";
}

void loadTemplates()
{
    templateTable.clear();

    fs::path tmpfile = getAppDataDir() / TMP_FILE_NAME;
    if (!fs::exists(tmpfile))
        return;

    std::ifstream file(tmpfile);
    std::string line;

    while (std::getline(file, line))
    {
        size_t pos = line.find(TMP_VAL_DELIM);
        if (pos == std::string::npos)
            continue; 

        std::string name  = line.substr(0, pos);
        std::string value = line.substr(pos + strlen(TMP_VAL_DELIM));

        templateTable.emplace_back(name, value);
    }
}

bool templateExists(const std::string& name)
{
    for (const auto& pair : templateTable)
    {
        if (pair.first == name)
            return true;
    }
    return false;
}

const std::vector<std::pair<std::string, std::string>>& getTemplateTable()
{
    loadTemplates();
    return templateTable;
}