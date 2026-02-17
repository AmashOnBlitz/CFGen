#pragma once 
#include <string>
#include <vector>
#include "DB.h"

std::string RegisterTemp(int argc, char const *argv[]);
std::string DeleteTemp(int argc, char const* argv[]);
std::string ShowAllTemplates();
const std::string GetTemplateRecordLocation();
bool templateExits(std::string name);
std::string getTemplateScript(std::string name, bool& ok);