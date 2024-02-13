/*
search content in a file with regex
created by clyde (Sun, 28 Jan 2024 10:29:55 CET)
edited by clyde (Tue, 13 Feb 2024 12:11:19 CET)
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
    const char *regex_pattern;
    FILE *file;

    if (argc == 2) {
        // If only one argument is provided, assume it's the regex pattern
        regex_pattern = argv[1];
        file = stdin;
    } else if (argc == 3) {
        // If two arguments are provided, assume the first is the regex pattern and the second is the file path
        regex_pattern = argv[1];
        const char *file_path = argv[2];
        file = fopen(file_path, "r");
        if (file == NULL) {
            perror("Error");
            return 1;
        }
    } else {
        printf("Usage: %s <regex_pattern> [file_path]\n", argv[0]);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    regex_t regex;

    if (regcomp(&regex, regex_pattern, REG_EXTENDED) != 0) {
        printf("Failed to compile regular expression.\n");
        if (file != stdin) {
            fclose(file);
        }
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
    if (file != stdin) {
        fclose(file);
    }

    return 0;
}
