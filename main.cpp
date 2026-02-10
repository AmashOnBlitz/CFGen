#include <iostream>
#include "globals.h"

#define COMMAND_HELP            "-h"
#define COMMAND_GEN             "-gen"
#define COMMAND_REGISTER_MACRO  "-reg"
int main(int argc, char const *argv[])
{
    if (argc < NO_OF_ARGUMENTS(1)){
        std::cout << 
        Constants::Instance().GetErrorString(ERRORCODE_NO_ARGUMENTS_SPECIFIED)
        << std::endl;
        return 0;
    }else{
        std::string cmd = argv[1];
        if (cmd == COMMAND_HELP){
            std::cout  <<
            Constants::Instance().GetString(STRINGCODE_HELP)
            << std::endl;
        }
        else if(cmd == COMMAND_REGISTER_MACRO){
            if (argc < NO_OF_ARGUMENTS(3)){
                std::cout << 
                Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_REG) 
                << std::endl;
            }
        }
        else if(cmd == COMMAND_GEN){
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
