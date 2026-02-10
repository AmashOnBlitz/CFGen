#include "strings.h"
#include <cstdlib>
#include <cstring>

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
        "-gen <file-name> <template-name>\n"
        "example : -gen header.h myHeaderTemplate\n"
        "Use -h for help";

    RETURN_ERR_STRING(errStr);
}

char *GetInsufficientArgumentsForRegErrorString()
{
    static const char errStr[] =
        "Insufficient Arguments For Registering A Macro!\n"
        "Least no of arguments are 3\n"
        "-reg <macro-name> <macro-value>\n"
        "example : -reg Name Foo_Bar_Baz\n"
        "Use -h for help";

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
        "  -h                           Show this help message\n"
        "  -gen <name> <tpl>            Generate a file using a template\n"
        "  -reg <macro-name> <value>    To Register A Macro"
        "\n"
        "Examples:\n"
        "  cfgen -h\n"
        "  cfgen -gen main.c mainTemplate\n";

    RETURN_ERR_STRING(helpStr);
}

