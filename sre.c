/*
search content in a file with regex
created by clyde (Sun, 28 Jan 2024 10:29:55 CET)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define BUFFER_SIZE 1024

void print_highlighted(const char *line, regmatch_t *matches, size_t num_matches) {
    size_t index = 0;
    for (size_t i = 0; i < num_matches; i++) {
        size_t start = matches[i].rm_so;
        size_t end = matches[i].rm_eo;

        while (index < start) {
            putchar(line[index]);
            index++;
        }

        printf("\033[1;31m");
        while (index < end) {
            putchar(line[index]);
            index++;
        }
        printf("\033[0m");
    }

    while (line[index] != '\0') {
        putchar(line[index]);
        index++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <regex_pattern> <file_path>\n", argv[0]);
        return 1;
    }

    const char *regex_pattern = argv[1];
    const char *file_path = argv[2];

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    regex_t regex;

    if (regcomp(&regex, regex_pattern, REG_EXTENDED) != 0) {
        printf("Failed to compile regular expression.\n");
        fclose(file);
        return 1;
    }

    int found_match = 0;

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        regmatch_t matches[1];
        if (regexec(&regex, buffer, 1, matches, 0) == 0) {
            if (found_match) {
                putchar('\n');
            }
            print_highlighted(buffer, matches, 1);
            found_match = 1;
        }
    }

    regfree(&regex);
    fclose(file);

    return 0;
}

