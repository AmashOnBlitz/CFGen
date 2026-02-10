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
        "<command> <file-name> <template-name>\n"
        "Use -h for help";

    RETURN_ERR_STRING(errStr);
}
