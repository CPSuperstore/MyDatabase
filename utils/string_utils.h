//
// Created by Christopher on 2021-10-08.
//

#ifndef MYDATABASE_STRING_UTILS_H
#define MYDATABASE_STRING_UTILS_H

#endif //MYDATABASE_STRING_UTILS_H

int count_chars(char* s, char c){
    return *s == '\0' ? 0 : count_chars(s + 1, c) + (*s == c);
}

int string_index(char* s, char c){
    char *e;

    e = strchr(s, c);
    return (int)(e - s);
}

int contains(char *haystack, char *needle){
    return strstr(haystack, needle) != NULL;
}
