#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <dirent.h>
#include <string.h>

void get_files(char buf[], char *location) {
    // buf[1024]
    DIR *dp = opendir(location);

    if (dp) {
        struct dirent *dir;
        while (dir = readdir(dp)) {
            size_t len = strlen(dir->d_name);
            if (dir->d_name[len-1] == 'c') {
                char file[len+1];
                strcpy(file, dir->d_name);
                file[len-2] = '\0';
                if (buf) {
                    strcat(buf, file);
                    strcat(buf, "\n");
                }
                printf("✔️  Loaded: %s\n", file);
            }
        }
        closedir(dp);
    }
    else printf("❌  Failed to open %s\n", location);

    return;
}
