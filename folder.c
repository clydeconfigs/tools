/*
create a folder
created by clyde (Sun, 28 Jan 2024 10:29:55 CET)
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <dir_name1> [<dir_name2> <dir_name3> ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        const char *dir_name = argv[i];
        int status = mkdir(dir_name, S_IRWXU);

        if (status == 0) {
            printf("Directory \"%s\" created successfully.\n", dir_name);
        } else {
            printf("Failed to create directory \"%s\".\n", dir_name);
        }
    }

    return 0;
}
