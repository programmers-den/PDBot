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
            size_t len = strlen(dir->d_name)-1;
            if (dir->d_name[len] == 'c') {
                char *file = malloc(len);
                strcpy(file, dir->d_name);
                file[len-1] = '\0';
                printf("Loaded: %s\n", file);
            }
        }
        closedir(dp);
    }
    else printf("Failed to open %s\n", location);

    return cmd;
}
