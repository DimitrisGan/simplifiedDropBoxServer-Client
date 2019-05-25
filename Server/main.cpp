#include <iostream>
#include "assistantFunctions.h"
#include "serverProtocol.h"

#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <arpa/inet.h>


void child_server(int newsock,Protocol prot);

void sigchld_handler (int sig);


int diavasa=0;



int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;

    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    cout<<argmKeeper.portNumber<<endl;


    Protocol prot(argmKeeper);



    int             port, sock, newsock;
    struct sockaddr_in server, client;
    socklen_t clientlen;

    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    struct hostent *rem;

//    if (argc != 2) {
//        printf("Please give port number\n");exit(1);}
//    port = atoi(argv[1]);

    port = argmKeeper.portNumber.to_int();

    /* Reap dead children asynchronously */
    signal(SIGCHLD, sigchld_handler);
    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");
    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */
    /* Bind socket to address */
    if (bind(sock, serverptr, sizeof(server)) < 0)
        perror_exit("bind");
    /* Listen for connections */
    if (listen(sock, 5) < 0) perror_exit("listen");
    printf("Listening for connections to port %d\n", port);
    while (true) {
        /* accept connection */
        if ((newsock = accept(sock, clientptr, &clientlen)) < 0) perror_exit("accept");


        printf("Accepted connection\n");
        switch (fork()) {    /* Create child for serving client */
            case -1:     /* Error */
                perror("fork"); break;
            case 0:	     /* Child process */
                close(sock); child_server(newsock,prot);
                exit(0);
        }
        close(newsock); /* parent closes socket to client            */
        /* must be closed before it gets re-assigned */


        printf("Accepted connection\n");
//                 child_server(newsock);close(sock);
        /* must be closed before it gets re-assigned */

    }

}

void child_server(int newsock,Protocol prot) {
    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    bool flagLOG_ON     = false;
    bool flagGET_CLIENTS= false;
    bool flagLOG_OFF    = false;

    while(read(newsock, buf, 1) > 0) {  /* Receive 1 char */
        printf("diavasa %d bytes\n", ++diavasa);

        instruction += buf;

        cout << "To instruction exei timh = " << instruction << endl;
        if (instruction == "LOG_ON") {
            flagLOG_ON = true;
            cout << instruction;
            break;
        }

        if (instruction == "GET_CLIENTS") {
            flagGET_CLIENTS = true;
            cout << instruction;
            break;
        }

        if (instruction == "LOG_OFF") {
            flagLOG_OFF = true;
            cout << instruction;
            break;
        }


    }


    if (flagLOG_ON)
        prot.recv_LOG_ON(newsock);


}




/* Wait for all dead child processes */
void sigchld_handler (int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
