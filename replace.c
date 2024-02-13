/*
replace content in a file with regex
created by clyde (Sun, 28 Jan 2024 10:40:37 CET)
edited by clyde (Tue, 13 Feb 2024 12:11:19 CET)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define BUFFER_SIZE 1024

void replace_substring(char *str, const char *find, const char *replace) {
    char *pos, *temp;
    int find_len = strlen(find);

    while ((pos = strstr(str, find)) != NULL) {
        temp = strdup(pos + find_len);
        strcpy(pos, replace);
        strcat(pos, temp);
        free(temp);
    }
}

void replace_regex(char *str, const char *regex_pattern, const char *replace) {
    regex_t regex;
    regcomp(&regex, regex_pattern, REG_EXTENDED);

    regmatch_t matches[1];
    while (regexec(&regex, str, 1, matches, 0) == 0) {
        int start = matches[0].rm_so;
        int end = matches[0].rm_eo;

        char *temp = strdup(str + end);
        strncpy(str + start, replace, BUFFER_SIZE - start);
        strncat(str, temp, BUFFER_SIZE - start);
        free(temp);
    }

    regfree(&regex);
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Usage: %s [-i] <find_string/regex_pattern> <replace_string> [file_path]\n", argv[0]);
        return 1;
    }

    int in_place = 0;
    int arg_offset = 1;

    if (argc == 4 && strcmp(argv[1], "-i") == 0) {
        in_place = 1;
        arg_offset = 2;
    }

    const char *find = argv[arg_offset];
    const char *replace = argv[arg_offset + 1];

    FILE *file;
    char buffer[BUFFER_SIZE];
    char *file_content = malloc(BUFFER_SIZE * sizeof(char));
    file_content[0] = '\0';

    if (arg_offset + 2 < argc) {
        const char *file_path = argv[arg_offset + 2];
        file = fopen(file_path, "r");
        if (file == NULL) {
            perror("Error");
            free(file_content);
            return 1;
        }

        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            strcat(file_content, buffer);
        }

        fclose(file);
    } else {
        while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            strcat(file_content, buffer);
        }
    }

    if (strstr(find, "\\") != NULL) {
        replace_regex(file_content, find, replace);
    } else {
        replace_substring(file_content, find, replace);
    }

    if (in_place) {
        file = fopen(argv[arg_offset + 2], "w");
        if (file == NULL) {
            perror("Error");
            free(file_content);
            return 1;
        }

        fputs(file_content, file);
        fclose(file);
    } else {
        printf("%s", file_content);
    }

    free(file_content);

    return 0;
}
