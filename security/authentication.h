//
// Created by Christopher on 2021-10-08.
//

#ifndef MYDATABASE_AUTHENTICATION_H
#define MYDATABASE_AUTHENTICATION_H

#endif //MYDATABASE_AUTHENTICATION_H

int authenticate_user(const char* username, const char* password){
    char path[256];
    char query[256];
    char check[256];
    char data[2048];

    snprintf(path, sizeof(path), "databases%susers.csv", PATH_SEP);

    snprintf(query, sizeof(query), "username=%s", username);
    snprintf(check, sizeof(check), "%s,%s", username, password);

    fetch_from_file(path, query, data);

    return contains(data, check);
}
