#include "globals.h"

#define INTERNAL_ERR_STR_NOT_FOUND "INTERNAL ERROR : ERR String Not Found !"

[[nodiscard]]
std::string fetchStrFrmChar(char* _c){
    std::string s;
    s = _c;
    free(_c);
    return s;
}

std::string Constants::GetErrorString(ERRORCODE errC)
{
    std::string errString = INTERNAL_ERR_STR_NOT_FOUND;
    switch (errC)
    {
    case ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN:{
        errString = fetchStrFrmChar(
            GetInsufficientArgumentsForGenErrorString()
        );
        break;
    }
    case ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG:{
        errString = fetchStrFrmChar(
            GetInsufficientArgumentsForRegMacErrorString()
        );
        break;
    }
    case ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG:{
        errString = fetchStrFrmChar(
            GetInsufficientArgumentsForRegTempErrorString()
        );
        break;
    }
    case ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG:{
        errString = fetchStrFrmChar(
            GetFileDoesNotExitForTempGenErrorString()
        );
        break;
    }
    case ERRORCODE_NO_ARGUMENTS_SPECIFIED:{
        errString = fetchStrFrmChar(
            GetNoArgumentsErrorString()
        );
        break;
    }
    
    default: [[unlikely]]
        break;
    }
    return errString;
}
std::string Constants::GetString(STRINGCODE strC)
{
    std::string str = INTERNAL_ERR_STR_NOT_FOUND;

    switch (strC)
    {
    case STRINGCODE_HELP:{
        str = fetchStrFrmChar(GetStringHelp());
        break;
    }
    default:
        break;
    }
    return str;
}
Constants::Constants()
{
}

Constants::~Constants()
{
}

Constants &Constants::Instance()
{
    static Constants c;
    return c;
}
