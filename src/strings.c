#include "strings.h"
#include <cstdlib>
#include <cstring>
#include "commands.h"

static char* assignToCharBuff(const char* str)
{
    if (!str) return NULL;

    size_t len = strlen(str) + 1;
    char* buff = (char*)malloc(len);
    if (!buff) return NULL;

    memcpy(buff, str, len);
    return buff;
}
#define RETURN_STRING(str) return assignToCharBuff(str)

char *GetUnknownCommandErrorString()
{
    static const char errStr[] = 
    "Unknown Argument Specified!\n"
    "Use -h for Help\n";
    RETURN_STRING(errStr);
}

char *GetUnknownSubCommandErrorString()
{
    static const char errStr[] = 
    "Unknown Sub Command Specified!\n"
    "Use -h for Help\n";
    RETURN_STRING(errStr);
}

char *GetNoArgumentsErrorString()
{
    static const char errStr[] =
        "No Arguments Specified\n"
        "Use -h for Help";

    RETURN_STRING(errStr);
}

char* GetInsufficientArgumentsForGenErrorString()
{
    static const char errStr[] =
        "Insufficient Arguments For Generating A File!\n"
        "Least no of arguments are 3\n"
        COMMAND_GEN
        " <file-name> <template-name>\n"
        "example : "
        COMMAND_GEN
        " header.h myHeaderTemplate\n"
        "Use -h for help";

    RETURN_STRING(errStr);
}

char *GetInsufficientArgumentsForRegMacErrorString()
{
    static const char errStr[] =
        "Insufficient Arguments For Registering A Macro!\n"
        "Least no of arguments are 3\n"
        COMMAND_REGISTER_TRIMMED
        " -m <macro-name> <macro-value>\n"
        "example : "
        COMMAND_REGISTER_TRIMMED
        " -m Name Foo_Bar_Baz\n"
        "Use -h for help";

    RETURN_STRING(errStr);
}

char *GetInsufficientArgumentsForRegTempErrorString()
{
    static const char errStr[] = 
    "Insufficient Arguments For Registering A Template!\n"
        "Least no of arguments are 3\n"
        COMMAND_REGISTER_TRIMMED
        " -t <template-name> <template-file-name>\n"
        "example : "
        COMMAND_REGISTER_TRIMMED
        " -t Temp1 tempheader.h\n"
        "Use -h for help";
        RETURN_STRING(errStr);
}

char *GetInsufficientArgumentsForDelTempErrorString()
{
    static const char errStr[] = 
    "Insufficient Arguments For Deleting A Template!\n"
        "Least no of arguments are 3\n"
        "---\n"
        COMMAND_DELETE
        " -t <template-name> \n"
        "---\n"
        "example : "
        COMMAND_DELETE
        " -t myTemplate\n"
        "Use -h for help";
        RETURN_STRING(errStr);
}

char *GetInsufficientArgumentsNoSubCommandErrorString()
{
    static const char errStr[] = 
    "Insufficient Arguments For This Command!\n"
    "This Command Requires A Sub Command\n"
    "arguments : <command> [sub-command] [options]\n";
    "Use -h for help\n";
    RETURN_STRING(errStr);
}

char *GetFileDoesNotExitForTempGenErrorString()
{
    static const char errStr[] = 
    "ERROR : Provided File Does Not Exist \n"
    "Template Registeration Failed ------ \n"
    "---\n"
    "Command Structure :\n"
    COMMAND_REGISTER_TRIMMED
    " -t <template-name> <template-file-name>\n";
    RETURN_STRING(errStr);
}

char *GetTempRegFileCannotBeCreatedErrorString()
{
    static const char errStr[] = 
    "ERROR : Cannot Create Template Record File!\n"
    "Please Check The Permissions Or Reinstall The App";
    RETURN_STRING(errStr);
}

char *GetTempAlrExistsErrorString()
{
    static const char errStr[] = 
    "ERROR : Template With This Name Already Exists!\n"
    "Please Use A Unique Name Or Remove This Template!";
    RETURN_STRING(errStr);
}

char *GetTempNotFoundForDeletionErrorString()
{
    static const char errStr[] = 
    "ERROR : Template With This Name Does Not Exist!\n"
    "Removal Aborted ---\n";
    RETURN_STRING(errStr);
}

char *GetCannotOpenProvidedFileErrorString()
{
    static const char errStr[] = 
    "ERROR : Cannot Open Provided File! \n";
    RETURN_STRING(errStr);
}

char *GetConnotCopyScriptFileErrorString()
{
    static const char errStr[] = 
    "ERROR : Cannot Open Provided File And Copy It To Required Destination! \n";
    RETURN_STRING(errStr);
}

char *GetTemplateNotfoundErrorString()
{
    static const char errStr[] = 
    "ERROR : The Requested Template Is Not Found In Template Records!\n";
    RETURN_STRING(errStr);
}

char *GetTemplateFileCannotBeWrittenErrorString()
{
    static const char errStr[] = 
    "ERROR : The Template Record File Cannot Be Opened!\n"
    "Checking Permissions Or Reinstalling The App May Help\n";
    RETURN_STRING(errStr);
}

char *GetCannotDeleteScriptFileErrorString()
{
    static const char errStr[] = 
    "ERROR : The Adjacent Template File Cannot Be Deleted!\n"
    "Checking Permissions Or Manually Deleting It May Help\n";
    RETURN_STRING(errStr);
}

char* GetStringHelp()
{
    static const char helpStr[] =
        "CFGen - Code/File Generator\n"
        "---------------------------\n"
        "Usage:\n"
        "  cfgen <command> [sub-command] [options]\n"
        "\n"
        "Commands:\n"
        "  -h                                              Show this help message\n"
        "  " COMMAND_GEN            " <name> <tpl>                            Generate a file using a template\n"
        "  " COMMAND_REGISTER       " -m <macro-name> <value>                 To Register A Macro\n"
        "  " COMMAND_REGISTER       " -t <template-name> <template-file-name> To Register A Template For File generation\n"
        "  " COMMAND_DELETE         " -m <macro-name>                         Delete a Macro\n"
        "  " COMMAND_DELETE         " -t <template-name>                      Delete a Template\n"
        "\n"
        "Examples:\n"
        "  cfgen -h\n"
        "  cfgen " COMMAND_GEN  " main.c mainTemplate\n"
        "  cfgen " COMMAND_REGISTER " -m Name Foo_Bar_Baz\n"
        "  cfgen " COMMAND_DELETE   " -m Name\n"
        "  cfgen " COMMAND_DELETE   " -t Temp1\n";

    RETURN_STRING(helpStr);
}

char *GetStringRegTempSuccessfull()
{
    static const char str[] = "Template Registeration Succesfull ------\n";
    RETURN_STRING(str);
}

char *GetStringDelTempSuccessfull()
{
    static const char str[] = "Template Deletion Succesfull ------\n";
    RETURN_STRING(str);
}
