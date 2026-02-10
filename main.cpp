#include <iostream>
#include "globals.h"

int main(int argc, char const *argv[])
{
    if (argc < NO_OF_ARGUMENTS(1)){
        std::cout << 
        Constants::Instance().GetErrorString(ERRORCODE_NO_ARGUMENTS_SPECIFIED)
        << std::endl;
        return 0;
    }
    if (argc < NO_OF_ARGUMENTS(3)){ // 1st arg is the exe name so ignore it
        std::cout << 
        Constants::Instance().GetErrorString(ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN) 
        << std::endl;
        return 0;
    }
    return 0;
}
