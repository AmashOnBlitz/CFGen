#pragma once
#include <string>
#include "strings.h"

constexpr int NO_OF_ARGUMENTS_MINUS_FIRST(int argc)
{
    return argc + 1;
}

constexpr int GET_ARGUMENT_INDEX(int argc)
{
    return argc + 1;
}


// Codes 0x0000f to 0x0200f Reserved For Error Strings
enum ERRORCODE : int {
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN        = 0x0001f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG   = 0x0002f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG   = 0x0004f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_DEL   = 0x0005f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_DEL   = 0x0006f,
    ERRORCODE_INSUFFICIENT_ARGUMENTS_NO_SUB         = 0x0007f,
    ERRORCODE_NO_ARGUMENTS_SPECIFIED                = 0x0016f,
    ERRORCODE_UNKNOWN_CMD_SPECIFIED                 = 0x0017f,
    ERRORCODE_UNKNOWN_SUB_CMD_SPECIFIED             = 0x0018f,

    ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG      = 0x0106f,
    ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE           = 0x0107f,
    ERRORCODE_TEMPLATE_ALR_EXISTS                   = 0x0108f,
    ERRORCODE_CANNOT_OPEN_PROVIDED_FILE             = 0x0109f,
    ERRORCODE_CANNOT_COPY_SCRIPT_FILE               = 0x0110f,
    ERRORCODE_TEMPLATE_NOT_FOUND_FOR_DEL            = 0x0111f,
    ERRORCODE_TEMPLATE_NOT_FOUND                    = 0x0112f,
    ERRORCODE_CANNOT_WRITE_TEMPLATE_FILE            = 0x0113f,
    ERRORCODE_CANNOT_DELETE_SCRIPT_FILE             = 0x0114f
};

// Codes 0x0200f to 0x0400f Reserved For Error Strings
enum STRINGCODE : int{
    STRINGCODE_HELP                                 = 0x0201f,
    STRINGCODE_TEMPLATE_REG_SUCCESSFULL             = 0x0202f,
    STRINGCODE_TEMPLATE_DEL_SUCCESSFULL             = 0x0203f
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
