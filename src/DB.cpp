#include "DB.h"
#include "filesys.hpp"
#include "globals.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

#define MACR_FILE_NAME  "macros.map"
#define TMP_FILE_NAME   "templates.map"
#define VAL_DELIM       ".key<=@=>val."

void loadTable(std::vector<std::pair<std::string,std::string>>& table, bool loadMacro){
    table.clear();
    fs::path filepath = getAppDataDir() / ((loadMacro)? MACR_FILE_NAME : TMP_FILE_NAME);
    if (!fs::exists(filepath))
        return;

    std::ifstream file(filepath);
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        continue;
        size_t pos = line.find(VAL_DELIM);
        if (pos == std::string::npos)
            continue; 

        std::string name  = line.substr(0, pos);
        std::string value = line.substr(pos + strlen(VAL_DELIM));

        table.emplace_back(name, value);
    }
}

#pragma region Template 

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
    
    bool isNewFile = !fs::exists(tmpfile);
    if (isNewFile) {
        std::ofstream headerFile(tmpfile, std::ios::out);
        if (!headerFile.is_open()) {
            return Constants::Instance().GetErrorString(
                ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE
            );
        }

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        headerFile << "===================================================================\n";
        headerFile << "Template Record File \n";
        headerFile << "# Created-On: "
                << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
                << "\n";
        headerFile << "# File-Format:\n";
        headerFile << "# <template_name>" << VAL_DELIM
                << "<script_filename>\n";
        headerFile << "===================================================================\n";
        headerFile << "\n";    

        headerFile.close();
    }

    std::fstream file;
    file.open(tmpfile, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE);
    }

    file << name << VAL_DELIM << inScript.filename().string() << "\n";
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
        file << pair.first << VAL_DELIM << pair.second << "\n";
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
    loadTable(templateTable,false);
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

const std::string getTemplateRecordFileDir(){
    fs::path dir = getAppDataDir() / TMP_FILE_NAME;
    if (!fs::exists(dir))
    return Constants::Instance().GetString(STRINGCODR_TEMP_FILE_DONT_EXIST_YET); 
    return dir.string();
} 


#pragma endregion Template

#pragma region Macro

std::vector<std::pair<std::string,std::string>> macroTable;
void loadMacros();
bool macroExists(const std::string& name);


std::string addMacro(std::string name, std::string value){
    fs::path macrFile = getAppDataDir() / MACR_FILE_NAME;
    if (!fs::exists(macrFile.parent_path()) && macrFile.has_parent_path()){
        if(!fs::create_directories(macrFile.parent_path())){
            return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_CREATE_MACRO_FILE);
        }
    }

    loadMacros();
    if(macroExists(name)){
        return Constants::Instance().GetErrorString(ERRORCODE_MARCO_ALR_EXISTS);
    }

    bool isNewFile = !fs::exists(macrFile);
    if (isNewFile) {
        std::ofstream headerFile(macrFile, std::ios::out);
        if (!headerFile.is_open()) {
            return Constants::Instance().GetErrorString(
                ERRORCODE_CANNOT_CREATE_MACRO_FILE
            );
        }

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        headerFile << "===================================================================\n";
        headerFile << "Macro Record File \n";
        headerFile << "# Created-On: "
                << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")
                << "\n";
        headerFile << "# File-Format:\n";
        headerFile << "# <macro_name>" << VAL_DELIM << "<macro_value>\n";
        headerFile << "===================================================================\n";
        headerFile << "\n";    

        headerFile.close();
    }

    std::fstream file;
    file.open(macrFile, std::ios::out | std::ios::app);
    if (!file.is_open()) {
        return Constants::Instance().GetErrorString(ERRORCODE_CANNOT_CREATE_MACRO_FILE);
    }

    file << name << VAL_DELIM << value << "\n";
    file.close();

    return "";
}

std::string removeMacro(const std::string& name)
{
    loadMacros();

    if (!macroExists(name)) {
        return Constants::Instance().GetErrorString(
            ERRORCODE_MACRO_NOT_FOUND_FOR_DEL
        );
    }

    for (auto it = macroTable.begin(); it != macroTable.end(); ++it) {
        if (it->first == name) {
            macroTable.erase(it);
            break;
        }
    }

    fs::path macrFile = getAppDataDir() / MACR_FILE_NAME;
    std::ofstream file(macrFile, std::ios::out);
    if (!file.is_open()) {
        return Constants::Instance().GetErrorString(
            ERRORCODE_CANNOT_WRITE_MACRO_FILE
        );
    }

    for (const auto& pair : macroTable) {
        file << pair.first << VAL_DELIM << pair.second << "\n";
    }

    file.close();
    return "";
}


void loadMacros(){
    loadTable(macroTable,true);
}

bool macroExists(const std::string& name){
    for (const auto& pair : macroTable)
    {
        if (pair.first == name)
            return true;
    }
    return false;
}

const std::string getMacroRecordFileDir(){
    fs::path dir = getAppDataDir() / MACR_FILE_NAME;
    if (!fs::exists(dir)){
        return Constants::Instance().GetString(STRINGCODR_MACR_FILE_DONT_EXIST_YET); 
    }

    return dir.string();
} 

const std::vector<std::pair<std::string, std::string>>& getMacroTable()
{
    loadMacros();
    return macroTable;
}

#pragma endregion Macro
