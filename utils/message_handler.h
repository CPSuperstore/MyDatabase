//
// Created by Christopher on 2021-10-08.
//

#ifndef MYDATABASE_MESSAGE_HANDLER_H
#define MYDATABASE_MESSAGE_HANDLER_H

#endif //MYDATABASE_MESSAGE_HANDLER_H


void handle_message(char **message, int arguments, char** response){
    for (int i = 0; i < arguments; ++i) {
        printf("%s ", message[i]);
    }
    printf("\n");

    if(strcmp(message[0], "read") == 0){
        fetch(message[1], message[2], message[3], response);
    }
}