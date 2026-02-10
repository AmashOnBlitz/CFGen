#pragma once
#include <string>
#include "strings.h"

constexpr int NO_OF_ARGUMENTS(int argc)
{
    return argc + 1;
}

constexpr int GET_ARGUMENT_INDEX(int argc)
{
    return argc + 1;
}


enum ERRORCODE : int {
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN        = 0x001f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG   = 0x002f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG   = 0x004f,
    ERRORCODE_NO_ARGUMENTS_SPECIFIED                = 0x005f,
    
    ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG      = 0x006f
};

enum STRINGCODE : int{
    STRINGCODE_HELP                             = 0x101f
};

class Constants
{
private:

public:
[[nodiscard]]
std::string GetErrorString(ERRORCODE errC);

[[nodiscard]]
std::string GetString(STRINGCODE strC);

public:
    Constants();
    ~Constants();
    static Constants& Instance();
};
