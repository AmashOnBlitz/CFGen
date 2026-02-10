#include "TemplateRegister.h"
#include <filesystem>
#include "globals.h"
#include <iostream>

namespace fs = std::filesystem;

std::string RegisterTemp(int argc, char const* argv[])
{
    if (argc < 4)
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG);

    if (!fs::exists(argv[3]))
        return Constants::Instance().GetErrorString(ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG);

    std::string str = std::string("Template Registeration Succesfull ------\n") + "Template : " + argv[2] + "\nFormat By : " + argv[3];
    return str;
}