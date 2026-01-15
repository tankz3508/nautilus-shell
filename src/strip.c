#include "../include/strip.h"

#include <string.h>

void strip_quotes(char* str) {
    unsigned long len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}
