#pragma once 
#include <string>

std::string RegisterMacr(int argc, char const *argv[]);
std::string DeleteMacr(int argc, char const* argv[]);
std::string ShowAllMacros();
const std::string GetMacroRecordLocation();