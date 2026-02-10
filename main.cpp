#include <iostream>
#include "globals.h"
#include "commands.h"
#include "TemplateRegister.h"

int main(int argc, char const *argv[])
{
    if (argc < NO_OF_ARGUMENTS(1)){
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
        else if (cmd == COMMAND_REGISTER_TEMPLATE_TRIMMED){ // TODO : ADD FUNCTIONALITY ---
            if (argc < NO_OF_ARGUMENTS(3)){
                std::cout <<
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG)
                << std::endl;
            }else{
                std::cout << RegisterTemp(argc,argv);
            }
        }
        else if(cmd == COMMAND_REGISTER_MACRO_TRIMMED){ // TODO : ADD FUNCTIONALITY...
            if (argc < NO_OF_ARGUMENTS(3)){
                std::cout << 
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG) 
                << std::endl;
            }
        }
        else if(cmd == COMMAND_GEN_TRIMMED){ // TODO : ADD FUNCTIONALITY...
            if (argc < NO_OF_ARGUMENTS(3)){ 
                std::cout << 
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN) 
                << std::endl;
                return 0;
            }
        }
    }
    return 0;
}
