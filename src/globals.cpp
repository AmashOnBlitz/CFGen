#include "globals.h"
#include <unordered_map>
#include <string>
#include "commands.h"

void Constants::initializeErrorStrings()
{
    errorMap[ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_GEN] = 
        "Insufficient Arguments For Generating A File!\n"
        "Least no of arguments are 3\n"
        COMMAND_GEN
        " <file-name> <template-name>\n"
        "example : "
        COMMAND_GEN
        " header.h myHeaderTemplate\n"
        "Use -h for help";

    errorMap[ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_REG] = 
        "Insufficient Arguments For Registering A Macro!\n"
        "Least no of arguments are 3\n"
        COMMAND_REGISTER_TRIMMED
        " -m <macro-name> <macro-value>\n"
        "example : "
        COMMAND_REGISTER_TRIMMED
        " -m Name Foo_Bar_Baz\n"
        "Use -h for help";

    errorMap[ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_MACR_DEL] = 
        "Insufficient Arguments For Deleting A Macro!\n"
        "Least no of arguments are 3\n"
        "---\n"
        COMMAND_DELETE_TRIMMED
        " -m <macro-name> \n"
        "---\n"
        "example : "
        COMMAND_DELETE_TRIMMED
        " -m myMacro\n"
        "Use -h for help";

    errorMap[ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_REG] = 
        "Insufficient Arguments For Registering A Template!\n"
        "Least no of arguments are 3\n"
        COMMAND_REGISTER_TRIMMED
        " -t <template-name> <template-file-name>\n"
        "example : "
        COMMAND_REGISTER_TRIMMED
        " -t Temp1 tempheader.h\n"
        "Use -h for help";

    errorMap[ERRORCODE_INSUFFICIENT_ARGUMENTS_FOR_TEMP_DEL] = 
        "Insufficient Arguments For Deleting A Template!\n"
        "Least no of arguments are 3\n"
        "---\n"
        COMMAND_DELETE_TRIMMED
        " -t <template-name> \n"
        "---\n"
        "example : "
        COMMAND_DELETE_TRIMMED
        " -t myTemplate\n"
        "Use -h for help";

    errorMap[ERRORCODE_INSUFFICIENT_ARGUMENTS_NO_SUB] = 
        "Insufficient Arguments For This Command!\n"
        "This Command Requires A Sub Command\n"
        "arguments : <command> [sub-command] [options]\n"
        "Use -h for help\n";

    errorMap[ERRORCODE_NO_ARGUMENTS_SPECIFIED] = 
        "No Arguments Specified\n"
        "Use -h for Help";

    errorMap[ERRORCODE_UNKNOWN_CMD_SPECIFIED] = 
        "Unknown Argument Specified!\n"
        "Use -h for Help\n";

    errorMap[ERRORCODE_UNKNOWN_SUB_CMD_SPECIFIED] = 
        "Unknown Sub Command Specified!\n"
        "Use -h for Help\n";

    errorMap[ERRORCODE_FILE_DOES_NOT_EXIST_FOR_TEMP_REG] = 
        "ERROR : Provided File Does Not Exist \n"
        "Template Registeration Failed ------ \n"
        "---\n"
        "Command Structure :\n"
        COMMAND_REGISTER_TRIMMED
        " -t <template-name> <template-file-name>\n";

    errorMap[ERRORCODE_CANNOT_CREATE_TEMPLATE_FILE] = 
        "ERROR : Cannot Create Template Record File!\n"
        "Please Check The Permissions Or Reinstall The App";

    errorMap[ERRORCODE_TEMPLATE_ALR_EXISTS] = 
        "ERROR : Template With This Name Already Exists!\n"
        "Please Use A Unique Name Or Remove This Template!";

    errorMap[ERRORCODE_CANNOT_OPEN_PROVIDED_FILE] = 
        "ERROR : Cannot Open Provided File! \n";

    errorMap[ERRORCODE_CANNOT_COPY_SCRIPT_FILE] = 
        "ERROR : Cannot Open Provided File And Copy It To Required Destination! \n";

    errorMap[ERRORCODE_TEMPLATE_NOT_FOUND] = 
        "ERROR : The Requested Template Is Not Found In Template Records!\n";

    errorMap[ERRORCODE_CANNOT_WRITE_TEMPLATE_FILE] = 
        "ERROR : The Template Record File Cannot Be Opened!\n"
        "Checking Permissions Or Reinstalling The App May Help\n";

    errorMap[ERRORCODE_CANNOT_DELETE_SCRIPT_FILE] = 
        "ERROR : The Adjacent Template File Cannot Be Deleted!\n"
        "Checking Permissions Or Manually Deleting It May Help\n";

    errorMap[ERRORCODE_TEMPLATE_NOT_FOUND_FOR_DEL] = 
        "ERROR : Template With This Name Does Not Exist!\n"
        "Removal Aborted ---\n";

    errorMap[ERRORCODE_CANNOT_CREATE_MACRO_FILE] = 
        "ERROR : Cannot Create Macro Record File!\n"
        "Please Check The Permissions Or Reinstall The App";

    errorMap[ERRORCODE_MARCO_ALR_EXISTS] =     
        "ERROR : Macro With This Name Already Exists!\n"
        "Please Use A Unique Name Or Remove This Macro!";
    
    errorMap[ERRORCODE_MACRO_NOT_FOUND_FOR_DEL] = 
        "ERROR : Macro With This Name Does Not Exist!\n"
        "Removal Aborted ---\n";

    errorMap[ERRORCODE_CANNOT_WRITE_MACRO_FILE] = 
        "ERROR : The Macro Record File Cannot Be Opened!\n"
        "Checking Permissions Or Reinstalling The App May Help\n";

    errorMap[ERRORCODE_CANNOT_OPEN_PROVIDED_SCRIPT_FILE] = 
    "ERROR : Cannot Read The Template Requested! \n"
    "Try Checking Permissions or Rebuiding Template";

    errorMap[ERRORCODE_TRANSFORM_DEPTH_EXCEEDED] =
        "[Warning] Too many nested transforms. Only ONE transform is allowed.";

    errorMap[ERRORCODE_INVALID_TRANSFORM_SYNTAX] =
        "[Error] Invalid transform format. Use: ${transform:${macro}}";

    errorMap[ERRORCODE_MACRO_UNMATCHED_BRACES] =
        "[Error] Macro braces are not balanced. Missing '${' or '}'.";

    errorMap[ERRORCODE_FAILED_TO_GEN_FILE] = 
        "[Error] Failed to create output file: ";

    errorMap[ERRORCODE_MACRO_NESTING_TOO_DEEP] = 
        "[Error] Macro nesting too deep (>";

    errorMap[ERRORCODE_FILE_ALR_EXISTS] = 
        "[Error] File Already Exists!\n"
        "Please delete the file to generate a new one";

    errorMap[ERRORCODE_FAILED_TO_FETCH_REMOTE_VERSION] =
        "[Error] Failed to fetch latest version information.\n"
        "Check your internet connection or GitHub availability.\n";

    errorMap[ERRORCODE_REMOTE_VERSION_EMPTY] =
        "[Error] Remote version data is invalid or empty.\n"
        "The version file may be corrupted or unreachable.\n";

    errorMap[ERRORCODE_VERSION_CHECK_NOT_AVAILABLE] =
        "[Warning] Version check is currently unavailable.\n"
        "Proceeding without update verification.\n";

}

void Constants::initializeGeneralStrings()
{
    stringMap[STRINGCODE_HELP] = 
        "CFGen - Code/File Generator\n"
        "---------------------------\n"
        "Usage:\n"
        "  cfgen <command> [sub-command] [options]\n"
        "\n"
        "Commands:\n"
        "  -h                                              Show This Help Message\n"
        "  " COMMAND_GEN            "    <file-name> <tpl>                    Generate A File Using A Template\n"
        "  " COMMAND_REGISTER       " -m <macro-name> <value>                 To Register A Macro\n"
        "  " COMMAND_REGISTER       " -t <template-name> <template-file-name> To Register A Template For File generation\n"
        "  " COMMAND_DELETE         " -m <macro-name>                         Delete A Macro\n"
        "  " COMMAND_DELETE         " -t <template-name>                      Delete A Template\n"
        "  " COMMAND_SHOW           " -t                                      Show Available Templates\n"
        "  " COMMAND_SHOW           " -m                                      Show Available Macroes\n"
        "  " COMMAND_SHOW           " -trdir                                  Show Template Records File Dir\n"
        "  " COMMAND_SHOW           " -mrdir                                  Show Macro Records File Dir\n"
        "  " COMMAND_VERSION        "                                         Shows Version\n"
        "\n"
        "Examples:\n"
        "  cfgen -h\n"
        "  cfgen " COMMAND_GEN  " main.c mainTemplate\n"
        "  cfgen " COMMAND_REGISTER " -m Name Foo_Bar_Baz\n"
        "  cfgen " COMMAND_DELETE   " -m Name\n"
        "  cfgen " COMMAND_DELETE   " -t Temp1\n";
        "  cfgen " COMMAND_VERSION  "\n";
        
    stringMap[STRINGCODE_TEMPLATE_REG_SUCCESSFULL] = "Template Registeration Succesfull ------\n";
    stringMap[STRINGCODE_TEMPLATE_DEL_SUCCESSFULL] = "Template Deletion Succesfull ------\n";
    stringMap[STRINGCODR_TEMP_FILE_DONT_EXIST_YET] = 
        "Template Record File Does Not Exist Yet\n"
        "Try Adding Some Templates! ---";
    stringMap[STRINGCODE_MACR_REG_SUCCESSFULL] = "Macro Registeration Succesfull ------\n"; 
    stringMap[STRINGCODE_MACR_DEL_SUCCESSFULL] = "Macro Deletion Succesfull ------\n";
    stringMap[STRINGCODR_MACR_FILE_DONT_EXIST_YET] = 
        "Macro Record File Does Not Exist Yet\n"
        "Try Adding Some Macro! ---";
    stringMap[STRINGCODE_MACR_NOT_REGISTERED] = "[Warning] Macro Not Registered : ";
    stringMap[STRINGCODE_FILE_GENERATED] = "[OK] File generated: ";

    stringMap[STRINGCODE_VERSION_HEADER] =
        "CFGen - Code/File Generator\n"
        "---------------------------\n";
    stringMap[STRINGCODE_VERSION_CHECKING] =
        "Checking for updates...\n";

    stringMap[STRINGCODE_VERSION_LOCAL] =
        "Installed Version : ";

    stringMap[STRINGCODE_VERSION_REMOTE] =
        "Latest Version    : ";

    stringMap[STRINGCODE_VERSION_UP_TO_DATE] =
        "Status            : You are using the latest version !\n";

    stringMap[STRINGCODE_VERSION_OUTDATED] =
        "Status            : Update available\n"
        "Visit: https://github.com/AmashOnBlitz/CFGen\n";

    stringMap[STRINGCODE_VERSION_BETA] =
        "Status            : You are running a newer (beta/dev) version!\n"
        "This version is ahead of the latest public release.\n";


}

std::string Constants::GetErrorString(ERRORCODE errC)
{
    auto it = errorMap.find(errC);
    if (it != errorMap.end()) {
        return it->second;
    }
    
    return "INTERNAL ERROR : Error String Not Found !\nError Code : " + std::to_string(static_cast<int>(errC));
}

std::string Constants::GetString(STRINGCODE strC)
{
    auto it = stringMap.find(strC);
    if (it != stringMap.end()) {
        return it->second;
    }

    return "INTERNAL ERROR : String Not Found !\nString Code : " + std::to_string(static_cast<int>(strC));
}

Constants::Constants()
{
    initializeErrorStrings();
    initializeGeneralStrings();
}

Constants::~Constants()
{
}

Constants &Constants::Instance()
{
    static Constants c;
    return c;
}