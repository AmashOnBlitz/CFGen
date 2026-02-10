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
    ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN    = 0x001f,
    ERRORCODE_NO_ARGUMENTS_SPECIFIED            = 0x002f,
};

class Constants
{
private:

public:
[[nodiscard]]
std::string GetErrorString(ERRORCODE errC);

public:
    Constants();
    ~Constants();
    static Constants& Instance();
};
