/*
create or modify the date of a 'thing', AKA file
created by clyde (Sun, 28 Jan 2024 10:29:55 +0100)
*/

#include <stdio.h>
#include <stdlib.h>
#include <utime.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> [<file2> <file3> ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        const char *file_path = argv[i];

        int file_exists = access(file_path, F_OK);

        if (file_exists == 0) {
            struct utimbuf new_times;
            new_times.actime = time(NULL);
            new_times.modtime = time(NULL);

            if (utime(file_path, &new_times) == 0) {
                printf("Timestamp of file \"%s\" modified successfully.\n", file_path);
            } else {
                printf("Failed to modify timestamp of file \"%s\".\n", file_path);
                perror("Error");
            }
        } else {
            int file_descriptor = open(file_path, O_CREAT, S_IRUSR | S_IWUSR);

            if (file_descriptor != -1) {
                printf("File \"%s\" created successfully.\n", file_path);
                close(file_descriptor);
            } else {
                printf("Failed to create file \"%s\".\n", file_path);
                perror("Error");
            }
        }
    }

    return 0;
}
