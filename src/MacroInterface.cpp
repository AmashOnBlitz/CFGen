#include "MacroInterface.h"
#include "globals.h"
#include "DB.h"

std::string RegisterMacr(int argc, char const *argv[])
{
    if (argc < 5){
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG);
    }

    std::string out = addMacro(argv[3],argv[4]);
    if (out != ""){
        return out; 
    }
    
    std::string str = Constants::Instance().GetString(STRINGCODE_MACR_REG_SUCCESSFULL) + "Macro : " + argv[3] + "\nValue  : " + argv[4];
    return str;
}

std::string DeleteMacr(int argc, char const *argv[])
{
    if (argc < 4){
        return Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_DEL);
    }

    std::string out = removeMacro(argv[3]);
    if (out != ""){
        return out; 
    }
    
    std::string str = Constants::Instance().GetString(STRINGCODE_MACR_DEL_SUCCESSFULL) + "Macro : " + argv[3] + " Removed!";
    return str;
}

std::string ShowAllMacros(){
    const auto& macros = getMacroTable();
    if (macros.empty()) {
        return "No macros available.";
    }

    std::string out = "Templates:\n";
    out += "----------------------------------------------------\n";
    out += "| #  | Macro Name            | Value               |\n";
    out += "----------------------------------------------------\n";

    int index = 1;
    for (const auto& macro : macros) {
        std::string name = macro.first;
        std::string path = macro.second;

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

const std::string GetMacroRecordLocation()
{
    return getMacroRecordFileDir();
}

bool macroExists(std::string name)
{
    return qureyMacroExists(name);
}
