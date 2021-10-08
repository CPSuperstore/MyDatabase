#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "utils/string_utils.h"
#include "disk/read_data.h"
#include "security/authentication.h"

#define DELIMITER " "

//the thread function
void *connection_handler(void *);


int main(int argc , char *argv[]) {
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1){
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) > 0){
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0){
        perror("accept failed");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc){
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *response , client_message[2000];

    int authenticated = 0;

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0){
        int argument_sections = count_chars(client_message, ' ') + 1;

        const char *arguments[argument_sections];
        char *arg = strtok(client_message, DELIMITER);

        int i = 0;
        while (arg != NULL) {
            arguments[i++] = arg;
            arg = strtok(NULL, DELIMITER);
        }

        if (authenticated == 0){
            if (strcmp(arguments[0], "auth") == 0){
                if(authenticate_user(arguments[1], arguments[2])){
                    authenticated = 1;
                    write(sock , "auth success" , strlen("auth success"));

                } else {
                    close(sock);
                    return 0;
                }

            } else {
                close(sock);
                return 0;
            }

        }
        else {
//            response = handle_message((char **) arguments, argument_sections);
//            printf("%s\n", response);
//            write(sock, response , strlen(response));
            write(sock, "done" , strlen("done"));
        }
    }

    if(read_size == 0){
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("recv failed");
    }

    //Free the socket pointer
    free(socket_desc);

    return 0;
}
