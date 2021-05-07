#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <dirent.h>
#include <string.h>

char *get_commands(char *location) {
    char *cmd = malloc(1024);
    DIR *dp = opendir(location);

    if (dp) {
        struct dirent *dir;
        while (dir = readdir(dp)) {
            if (dir->d_name[strlen(dir->d_name)-1] == 'c') {
                char *token = strtok(dir->d_name, ".c");
                while (token) {
                    strcat(cmd, token);
                    strcat(cmd, "\n");
                    printf("Loaded: %s\n", token);
                    token = strtok(NULL, ".c");
                }
            }
        }
        closedir(dp);
    }
    else printf("Failed to open %s\n", location);

    return cmd;
}
