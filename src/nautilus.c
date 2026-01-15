#include <bits/posix1_lim.h>
#define _POSIX_C_SOURCE 200809L
#define __USE_POSIX

#include <fcntl.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/parser.h"
#include "../include/strip.h"

int main() {
    while (true) {
        bool redir_found = false;
        char input[1024];
        char redir_file[128];

        memset(input, 0, sizeof(input));

        int i;

        char* home = getenv("HOME");
        char cwd[PATH_MAX];
        char hostname[HOST_NAME_MAX + 1];

        if (gethostname(hostname, HOST_NAME_MAX + 1) != 0)
            break;

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            if (strcmp(home, cwd) == 0) {
                printf("[%s@%s %s]> ", getlogin(), hostname, "~");
            } else {
                printf("[%s@%s %s]> ", getlogin(), hostname, strrchr(cwd, '/') + 1);
            }

        } else {
            perror("getcwd() error");
            return 1;
        }
        
        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = 0;

        char* tok = strtok(input, " ");
        char* args[64];
        memset(args, 0, sizeof(args));

        for (i = 0; tok != NULL; i++) {
            args[i] = tok;
            tok = strtok(NULL, " ");
        }

        args[i] = NULL;
        expand_env_vars(args);

        char** args_right = NULL;

        for (i = 0; args[i] != NULL; i++) {
            strip_quotes(args[i]);

            if (strcmp(">", args[i]) == 0) {
                redir_found = true;
                strcpy(redir_file, args[i + 1]);
                args[i] = NULL;
                break;
            } else if (strcmp("|", args[i]) == 0) {
                args[i] = NULL;
                args_right = &args[i + 1];
            }
        }

        if (strcmp("cd", args[0]) == 0) {
            if (args[1] == NULL) {
                chdir(getenv("HOME"));
            } else {
                if (chdir(args[1]) != 0)
                    perror("cd");
            }

            continue;
        }

        if (strcmp("exit", args[0]) == 0) {
            break;
        }

        if (args_right == NULL) {
            pid_t procid = fork();

            if (procid == 0) {
                if (redir_found) {
                    int file_desc = open(redir_file, O_WRONLY | O_CREAT | O_TRUNC);
                    dup2(file_desc, 1);

                    if (execvp(args[0], args) != 0) {
                        perror("shell");
                        exit(1);
                    }
                } else {
                    if (execvp(args[0], args) != 0) {
                        perror("shell");
                        exit(1);
                    }
                }
            } else {
                wait(NULL);
            }
        } else {
            int fd[2];
            pipe(fd);

            if (fork() == 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                close(fd[1]);

                if (execvp(args[0], args) != 0) {
                    perror("shell");
                    exit(1);
                }
            }

            if (fork() == 0) {
                dup2(fd[0], 0);

                if (redir_found) {
                    int file_desc = open(redir_file, O_WRONLY | O_CREAT | O_TRUNC);
                    if (file_desc < 0) {
                        perror("open");
                        exit(1);
                    }
                    dup2(file_desc, 1);
                    close(file_desc);
                }

                close(fd[0]);
                close(fd[1]);

                if (execvp(args_right[0], args_right) != 0) {
                    perror("shell");
                    exit(1);
                }
            }

            close(fd[0]);
            close(fd[1]);

            wait(NULL);
            wait(NULL);
        }
    }

    return EXIT_SUCCESS;
}
