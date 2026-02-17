#pragma once 
#include <string>
#include <vector>

bool queryTemplateExists(const std::string& name);
std::string addTemplate(std::string name, std::string value);
std::string removeTemplate(const std::string& name);
const std::vector<std::pair<std::string, std::string>>& getTemplateTable();
const std::string getTemplateRecordFileDir();
std::string readScript(std::string name, bool& ok);
bool qureyMacroExists(const std::string& name);
std::string addMacro(std::string name, std::string value);
std::string removeMacro(const std::string& name);
const std::string getMacroRecordFileDir();
const std::vector<std::pair<std::string, std::string>>& getMacroTable();
