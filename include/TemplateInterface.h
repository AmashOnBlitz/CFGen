#pragma once 
#include <string>

std::string RegisterTemp(int argc, char const *argv[]);
std::string DeleteTemp(int argc, char const* argv[]);
std::string ShowAllTemplates();
const std::string GetTemplateRecordLocation();