#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <dirent.h>
#include <string.h>

char *get_files(char *location) {
    char *files = malloc(1024);
    memset(files, 0, 1024);
    DIR *dp = opendir(location);

    if (dp) {
        struct dirent *dir;
        while (dir = readdir(dp)) {
            size_t len = strlen(dir->d_name);
            if (dir->d_name[len-1] == 'c') {
                char *file = malloc(len+1);
                memset(file, 0, len+1);
                strcpy(file, dir->d_name);
                file[len-2] = '\0';
                strcat(files, file);
                strcat(files, "\n");
                printf("✔️  Loaded: %s\n", file);
                free(file);
            }
        }
        closedir(dp);
    }
    else printf("❌  Failed to open %s\n", location);

    return files;
}
