#include "TemplateInterface.h"
#include <filesystem>
#include "globals.h"
#include <iostream>
#include "TemplateDb.h"

namespace fs = std::filesystem;

std::string RegisterTemp(int argc, char const* argv[])
{
    if (argc < 5){
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG);
    }
    if (!fs::exists(argv[4])){
        return Constants::Instance().GetErrorString(ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG);
    }

    std::string out = addTemplate(argv[3],argv[4]);
    if (out != ""){
        return out; 
    }
    
    std::string str = Constants::Instance().GetString(STRINGCODE_TEMPLATE_REG_SUCCESSFULL) + "Template : " + argv[3] + "\nFormat By : " + argv[4];
    return str;
}

std::string DeleteTemp(int argc, char const* argv[])
{
    if (argc < 4){
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_DEL);
    }

    std::string out = removeTemplate(argv[3]);
    if (out != ""){
        return out; 
    }
    
    std::string str = Constants::Instance().GetString(STRINGCODE_TEMPLATE_DEL_SUCCESSFULL) + "Template : " + argv[3] + " Removed!";
    return str;
}

std::string ShowAllTemplates()
{
    const auto& templates = getTemplateTable();
    if (templates.empty()) {
        return "No templates available.";
    }

    std::string out = "Templates:\n";
    out += "----------------------------------------------------\n";
    out += "| #  | Template Name         | File Path           |\n";
    out += "----------------------------------------------------\n";

    int index = 1;
    for (const auto& temp : templates) {
        std::string name = temp.first;
        std::string path = temp.second;

        if (name.length() > 20) name = name.substr(0, 17) + "...";
        if (path.length() > 20) path = path.substr(0, 17) + "...";

        out += "| " + std::to_string(index) + std::string(2 - std::to_string(index).length(), ' ')
             + " | " + name + std::string(20 - name.length(), ' ')
             + " | " + path + std::string(20 - path.length(), ' ') + " |\n";

        index++;
    }

    out += "----------------------------------------------------\n";
    return out;
}
