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
    errString = errString + "\nError Code : " + std::to_string(static_cast<int>(errC));
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
    case ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_DEL:{
        errString = fetchStrFrmChar(
            GetInsufficientArgumentsForDelTempErrorString()
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
    case ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE:{
        errString = fetchStrFrmChar(
            GetTempRegFileCannotBeCreatedErrorString()
        );
        break;
    }
    case ERRORCODE_CANNOT_OPEN_PROVIDED_FILE:{
        errString = fetchStrFrmChar(
            GetCannotOpenProvidedFileErrorString()
        );
        break;
    }
    case ERRORCODE_CANNOT_COPY_SCRIPT_FILE:{
        errString = fetchStrFrmChar(
            GetConnotCopyScriptFileErrorString()
        );
        break;
    }
    case ERRORCODE_TEMPLATE_NOT_FOUND:{
        errString = fetchStrFrmChar(
            GetTemplateNotfoundErrorString()
        );
        break;
    }
    case ERRORCODE_CANNOT_WRITE_TEMPLATE_FILE:{
        errString = fetchStrFrmChar(
            GetTemplateFileCannotBeWritten()
        );
        break;
    }
    case ERRORCODE_TEMPLATE_ALR_EXISTS:{
        errString = fetchStrFrmChar(
            GetTempAlrExistsErrorString()
        );
        break;
    }
    case ERRORCODE_CANNOT_DELETE_SCRIPT_FILE:{
        errString = fetchStrFrmChar(
            GetCannotDeleteScriptFile()
        );
        break;
    }
    case ERRORCODE_TEMPLATE_NOT_FOUND_FOR_DEL:{
        errString = fetchStrFrmChar(
            GetTemplateNotfoundErrorString()
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
    case STRINGCODE_TEMPLATE_REG_SUCCESSFULL:{
        str = fetchStrFrmChar(GetStringRegTempSuccessfull());
        break;
    }
    case STRINGCODE_TEMPLATE_DEL_SUCCESSFULL:{
        str = fetchStrFrmChar(GetStringDelTempSuccessfull());
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
