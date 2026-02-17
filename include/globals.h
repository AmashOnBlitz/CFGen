#pragma once
#include <string>
#include <unordered_map>

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
    ERRORCODE_CANNOT_DELETE_SCRIPT_FILE             = 0x0114f,
    ERRORCODE_CANNOT_CREATE_MACRO_FILE              = 0x0115f,
    ERRORCODE_MARCO_ALR_EXISTS                      = 0x0116f,
    ERRORCODE_MACRO_NOT_FOUND_FOR_DEL               = 0x0117f,
    ERRORCODE_CANNOT_WRITE_MACRO_FILE               = 0x0118f,
    ERRORCODE_CANNOT_OPEN_PROVIDED_SCRIPT_FILE      = 0x0119f,
    ERRORCODE_TRANSFORM_DEPTH_EXCEEDED              = 0x0120f,
    ERRORCODE_MACRO_UNMATCHED_BRACES                = 0x0130f,
    ERRORCODE_INVALID_TRANSFORM_SYNTAX              = 0x0131f,
    ERRORCODE_FAILED_TO_GEN_FILE                    = 0x0132f,
    ERRORCODE_MACRO_NESTING_TOO_DEEP                = 0x0133f,
    ERRORCODE_FILE_ALR_EXISTS                       = 0x0134f,
    ERRORCODE_FAILED_TO_FETCH_REMOTE_VERSION        = 0x0135f,
    ERRORCODE_REMOTE_VERSION_EMPTY                  = 0x0136f,
    ERRORCODE_VERSION_CHECK_NOT_AVAILABLE           = 0x0137f
};

// Codes 0x0200f to 0x0400f Reserved For Error Strings
enum STRINGCODE : int{
    STRINGCODE_HELP                                 = 0x0201f,
    STRINGCODE_TEMPLATE_REG_SUCCESSFULL             = 0x0202f,
    STRINGCODE_TEMPLATE_DEL_SUCCESSFULL             = 0x0203f,
    STRINGCODR_TEMP_FILE_DONT_EXIST_YET             = 0x0204f,
    STRINGCODE_MACR_REG_SUCCESSFULL                 = 0x0205f,
    STRINGCODE_MACR_DEL_SUCCESSFULL                 = 0x0206f,
    STRINGCODR_MACR_FILE_DONT_EXIST_YET             = 0x0207f,
    STRINGCODE_MACR_NOT_REGISTERED                  = 0x0208f,
    STRINGCODE_FILE_GENERATED                       = 0x0209f,
    STRINGCODE_VERSION_HEADER                       = 0x0210f,
    STRINGCODE_VERSION_UP_TO_DATE                   = 0x0211f,
    STRINGCODE_VERSION_OUTDATED                     = 0x0212f,
    STRINGCODE_VERSION_CHECKING                     = 0x0213f,
    STRINGCODE_VERSION_LOCAL                        = 0x0214f,
    STRINGCODE_VERSION_REMOTE                       = 0x0215f,
    STRINGCODE_VERSION_BETA                         = 0x0216f
};

class Constants
{
private:
std::unordered_map<ERRORCODE, std::string> errorMap;
std::unordered_map<STRINGCODE, std::string> stringMap;
void initializeErrorStrings();
void initializeGeneralStrings();
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
