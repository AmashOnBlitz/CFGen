#include "globals.h"

[[nodiscard]]
std::string fetchStrFrmChar(char* _c){
    std::string s;
    s = _c;
    free(_c);
    return s;
}

std::string Constants::GetErrorString(ERRORCODE errC)
{
    std::string errString = "INTERNAL ERROR : ERR String Not Found !";
    switch (errC)
    {
    case ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN:{
        errString = fetchStrFrmChar(
            GetInsufficientArgumentsForGenErrorString()
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
