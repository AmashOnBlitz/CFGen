#include <iostream>
#include "globals.h"
#include "commands.h"
#include "TemplateInterface.h"
#include "MacroInterface.h"

int main(int argc, char const *argv[])
{
    if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(1)){
        std::cout << 
        Constants::Instance().GetErrorString(ERRORCODE_NO_ARGUMENTS_SPECIFIED)
        << std::endl;
        return 0;
    }else{
        std::string cmd = argv[1];
        if (cmd == COMMAND_HELP_TRIMMED){ // Funtionality Complete ---
            std::cout  <<
            Constants::Instance().GetString(STRINGCODE_HELP)
            << std::endl;
        }
        else if (cmd == COMMAND_REGISTER_TRIMMED){ 
            if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(2)){
                std::cout <<
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_NO_SUB);
            }else{
                std::string subCmd = argv[2];
                if (subCmd == COMMAND_TEMPLATE_SUB){ 
                    if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(3)){
                        std::cout << 
                        Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG) 
                        << std::endl;
                    }
                    else{
                        std::cout << RegisterTemp(argc,argv);
                    }
                }
                else if (subCmd == COMMAND_MACRO_SUB){
                    if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(3)){
                        std::cout << 
                        Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG) 
                        << std::endl;
                    }
                    else{
                        std::cout << RegisterMacr(argc,argv);
                    }
                }
                else{
                    std::cout << Constants::Instance().GetErrorString(ERRORCODE_UNKNOWN_SUB_CMD_SPECIFIED); 
                }
            }
        }
        else if (cmd == COMMAND_DELETE_TRIMMED){ // Funtionality Complete ---
            if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(2)){
                std::cout <<
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_NO_SUB);
            }
            else{
                std::string subCmd = argv[2];
                if (subCmd == COMMAND_TEMPLATE_SUB){ 
                    if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(3)){
                        std::cout <<
                        Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_DEL)
                        << std::endl;
                    }
                    else{
                        std::cout << DeleteTemp(argc,argv);
                    }
                }
                else if (subCmd == COMMAND_MACRO_SUB){
                    if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(3)){
                        std::cout <<
                        Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_DEL)
                        << std::endl;
                    }else{
                        std::cout << DeleteMacr(argc,argv);
                    }
                }
                else{
                    std::cout << Constants::Instance().GetErrorString(ERRORCODE_UNKNOWN_SUB_CMD_SPECIFIED); 
                }
            }
        }
        else if (cmd == COMMAND_SHOW_TRIMMED){ // Funtionality Complete ---
            if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(2)){
                    std::cout <<
                    Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_NO_SUB);
                }
            else{
                std::string subCmd = argv[2];
                if (subCmd == COMMAND_TEMPLATE_SUB){
                    std::cout << ShowAllTemplates();
                }
                else if (subCmd == COMMAND_MACRO_SUB){
                    std::cout << ShowAllMacros();
                }
                else if (subCmd == COMMAND_TEMP_RC_DIR_SUB){
                    std::cout << GetTemplateRecordLocation();
                }
                else if (subCmd == COMMAND_MACR_RC_DIR_SUB){
                    std::cout << GetMacroRecordLocation();
                }
                else{
                    std::cout << Constants::Instance().GetErrorString(ERRORCODE_UNKNOWN_SUB_CMD_SPECIFIED); 
                }
            }
        }
        else if(cmd == COMMAND_GEN_TRIMMED){ // TODO : Functionality Yet To Be Implemented...
            if (argc < NO_OF_ARGUMENTS_MINUS_FIRST(3)){ 
                std::cout << 
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN) 
                << std::endl;
                return 0;
            }else{
                std::cout << "TODO: Yet To Be Implemented!" << std::endl;
            }
        }
        else {
            std::cout << Constants::Instance().GetErrorString(ERRORCODE_UNKNOWN_CMD_SPECIFIED);
        }
    }
    return 0;
}
