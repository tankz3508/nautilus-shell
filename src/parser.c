#include "../include/parser.h"

#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

void expand_env_vars(char* args[]) {
    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char* env = getenv(&args[i][1]);

            if (env != NULL)
                args[i] = env;
            else
                args[i] = "";
        }
    }
}
