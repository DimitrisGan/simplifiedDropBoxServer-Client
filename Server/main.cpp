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


void child_server(int newsock,Protocol &prot);

void sigchld_handler (int sig);







//int
//send_to_client(){}
//
//int broadcast_to_clients{}



int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;

    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    cout<<argmKeeper.portNumber<<endl;

    Protocol prot(argmKeeper);

    int port, sock;

    port = argmKeeper.portNumber.to_int();

    sock = make_socket(static_cast<uint16_t>(port));

    /* Listen for connections */
    if (listen(sock, 5) < 0) perror_exit("listen");
    printf("Listening for connections to port %d\n", port);

    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    socklen_t size;

    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (sock, &active_fd_set);

    while (true)
    {
        cout << "hi I am Server\n";

        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror ("select");
            exit (EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET (i, &read_fd_set)) {
                if (i == sock) {
                    /* Connection request on original socket. */
                    int newsock;
                    size = sizeof(clientname);
                    newsock = accept(sock, (struct sockaddr *) &clientname, &size);
                    if (newsock < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    fprintf(stderr,
                            "Server: connect from host %s, port %hd.\n",
                            inet_ntoa(clientname.sin_addr),
                            ntohs(clientname.sin_port));
                    FD_SET (newsock, &active_fd_set);
                } else {
                    /* Data arriving on an already-connected socket. */
                    if (read_from_client(i, prot) < 0) {
                        close(i);
                        FD_CLR (i, &active_fd_set);
                    }
                }
            }
        }
    }
}





void child_server(int newsock,Protocol &prot) {
    int diavasa=0;

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


    if (flagLOG_ON) {
        prot.recv_LOG_ON(newsock /*, 0, 0*/);
//        prot.broadcast_USER_ON();

    }


}






/* Wait for all dead child processes */
void sigchld_handler (int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
