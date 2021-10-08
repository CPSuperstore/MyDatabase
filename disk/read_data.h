//
// Created by Christopher on 2021-10-08.
//

#ifndef MYDATABASE_READ_DATA_H
#define MYDATABASE_READ_DATA_H

#endif //MYDATABASE_READ_DATA_H

#if defined _WIN32 || defined __CYGWIN__
#define PATH_SEP "\\"
#else
#define PATH_SEP "/"
#endif


int fetch_from_file(char* path, char* condition, char** result){
    FILE* stream = fopen(path, "r");
    int matches = 0;

    int search_index = -1;

    char line[1024];
    char value[256];

    while (fgets(line, 1024, stream)){
        if(search_index == -1){
            strcat(result, line);
        }

        line[strcspn(line, "\r\n")] = 0;

        char* tmp = strdup(line);

        int argument_sections = count_chars(line, ',') + 1;

        const char *arguments[argument_sections];
        char *arg = strtok(line, ",");

        int i = 0;
        while (arg != NULL) {
            arguments[i++] = arg;
            arg = strtok(NULL, ",");
        }

        if(search_index == -1){
            int equals = string_index(condition, '=');

            char column[256];
            memcpy(column, &condition[0], equals);
            memcpy(value, &condition[equals + 1], 256);

            for (i = 0; i < argument_sections; ++i) {
                if (strcmp(arguments[i], column) == 0){
                    search_index = i;
                }
            }
            continue;
        }

        if(strcmp(arguments[search_index], value) == 0){
            for (int j = 0; j < argument_sections; ++j) {
                strcat(result, arguments[j]);

                if(j != argument_sections - 1){
                    strcat(result, ",");
                }
            }
            strcat(result, "\n");

            matches ++;
        }
        free(tmp);
    }
    return matches;
}

int fetch(char* database, char* table, char* condition, char** result){
    char path[256];
    snprintf(path, sizeof(path), "databases%s%s%s%s.csv", PATH_SEP, database, PATH_SEP, table);

    return fetch_from_file(path, condition, result);
}