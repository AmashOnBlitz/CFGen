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
#define RETURN_ERR_STRING(str) return assignToCharBuff(str)

char* GetNoArgumentsErrorString()
{
    static const char errStr[] =
        "No Arguments Specified\n"
        "Use -h for Help";

    RETURN_ERR_STRING(errStr);
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

    RETURN_ERR_STRING(errStr);
}

char *GetInsufficientArgumentsForRegMacErrorString()
{
    static const char errStr[] =
        "Insufficient Arguments For Registering A Macro!\n"
        "Least no of arguments are 3\n"
        COMMAND_REGISTER_MACRO
        " <macro-name> <macro-value>\n"
        "example : "
        COMMAND_REGISTER_MACRO
        " Name Foo_Bar_Baz\n"
        "Use -h for help";

    RETURN_ERR_STRING(errStr);
}

char *GetInsufficientArgumentsForRegTempErrorString()
{
    static const char errStr[] = 
    "Insufficient Arguments For Registering A Template!\n"
        "Least no of arguments are 3\n"
        COMMAND_REGISTER_TEMPLATE
        " <template-name> <template-file-name>\n"
        "example : "
        COMMAND_REGISTER_TEMPLATE
        " Temp1 tempheader.h\n"
        "Use -h for help";
        RETURN_ERR_STRING(errStr);
}

char *GetFileDoesNotExitForTempGenErrorString()
{
    static const char errStr[] = 
    "ERROR : Provided File Does Not Exist \n"
    "Template Registeration Failed ------ \n";

    RETURN_ERR_STRING(errStr);
}

char* GetStringHelp()
{
    static const char helpStr[] =
        "CFGen - Code/File Generator\n"
        "---------------------------\n"
        "Usage:\n"
        "  cfgen <command> [options]\n"
        "\n"
        "Commands:\n"
        "  -h                                              Show this help message\n"
        "  " COMMAND_GEN " <name> <tpl>                            Generate a file using a template\n"
        "  " COMMAND_REGISTER_MACRO " <macro-name> <value>                    To Register A Macro\n"
        "  " COMMAND_REGISTER_TEMPLATE " <template-name> <template-file-name>    To Register A Template For File generation"
        "\n"
        "Examples:\n"
        "  cfgen -h\n"
        "  cfgen " COMMAND_GEN " main.c mainTemplate\n"
        "  cfgen " COMMAND_REGISTER_MACRO " Name Foo_Bar_Baz\n";

    RETURN_ERR_STRING(helpStr);
}

