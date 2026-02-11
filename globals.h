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
    
    ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG      = 0x006f,
    ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE           = 0x007f,
    ERRORCODE_TEMPLATE_ALR_EXISTS                   = 0x008f,
    ERRORCODE_CANNOT_OPEN_PROVIDED_FILE             = 0x009f,
    ERRORCODE_CANNOT_COPY_SCRIPT_FILE               = 0x010f,
    ERRORCODE_TEMPLATE_NOT_FOUND                    = 0x011f,
    ERRORCODE_CANNOT_WRITE_TEMPLATE_FILE            = 0x012f,
    ERRORCODE_CANNOT_DELETE_SCRIPT_FILE             = 0x013f
};

enum STRINGCODE : int{
    STRINGCODE_HELP                             = 0x101f,
    STRINGCODE_TEMPLATE_REG_SUCCESSFULL         = 0x102f
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
