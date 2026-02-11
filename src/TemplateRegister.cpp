#include "TemplateRegister.h"
#include <filesystem>
#include "globals.h"
#include <iostream>
#include "TemplateDb.h"

namespace fs = std::filesystem;

std::string RegisterTemp(int argc, char const* argv[])
{
    if (argc < 4){
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG);
    }
    if (!fs::exists(argv[3])){
        return Constants::Instance().GetErrorString(ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG);
    }

    std::string out = addTemplate(argv[2],argv[3]);
    if (out != ""){
        return out; 
    }
    
    std::string str = Constants::Instance().GetString(STRINGCODE_TEMPLATE_REG_SUCCESSFULL) + "Template : " + argv[2] + "\nFormat By : " + argv[3];
    return str;
}

std::string DeleteTemp(int argc, char const* argv[])
{
    if (argc < 3){
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_DEL);
    }

    std::string out = removeTemplate(argv[2]);
    if (out != ""){
        return out; 
    }
    
    std::string str = Constants::Instance().GetString(STRINGCODE_TEMPLATE_DEL_SUCCESSFULL) + "Template : " + argv[2] + " Removed!";
    return str;
}