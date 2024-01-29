/*
search content in a file with regex
created by clyde (Mon, 29 Jan 2024 19:26:49 CET)
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void listFiles(const char *path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        struct stat statbuf;
        struct passwd *pw;
        struct group *gr;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
        printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
        printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
        printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
        printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
        printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
        printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
        printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
        printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
        printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %2lu", statbuf.st_nlink);

        if ((pw = getpwuid(statbuf.st_uid)) != NULL) {
            printf(" %s", pw->pw_name);
        } else {
            printf(" %d", statbuf.st_uid);
        }

        if ((gr = getgrgid(statbuf.st_gid)) != NULL) {
            printf(" %s", gr->gr_name);
        } else {
            printf(" %d", statbuf.st_gid);
        }

        printf(" %8ld", statbuf.st_size);

        char time_str[80];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&statbuf.st_mtime));
        printf(" %s", time_str);

        printf(" %s\n", entry->d_name);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path;

    if (argc == 1) {
        path = ".";
    } else if (argc == 2) {
        path = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    listFiles(path);

    return 0;
}
