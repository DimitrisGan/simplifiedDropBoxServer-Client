#include <iostream>
#include "assistantFunctions.h"
#include "serverProtocol.h"
#include "socketManipulation.h"

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



void sigchld_handler (int sig);


int
read_request_from_client_and_respond(int filedes, Protocol &prot);


//int
//send_to_client(){}
//
//int broadcast_to_clients{}



int main(int argc, char **argv) {


    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    cout<<argmKeeper.portNumber<<endl;

    Protocol prot(argmKeeper);

    int port, sock;

    port = argmKeeper.portNumber.to_int();

    sock = make_socket_and_bind(static_cast<uint16_t>(port));

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

        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
            perror_exit("select");

        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET (i, &read_fd_set)) {
                if (i == sock) {
                    /* Connection request on original socket. */
                    int newsock;
                    size = sizeof(clientname);
                    newsock = accept(sock, (struct sockaddr *) &clientname, &size);
                    if (newsock < 0)
                       perror_exit("accept");


//                    fprintf(stderr,
//                            "Server: connect from host %s, port %d.\n",
//                            inet_ntoa(clientname.sin_addr),
//                            ntohs(clientname.sin_port));
                    FD_SET (newsock, &active_fd_set);
                } else {
                    /* Data arriving on an already-connected socket. */
                    if (read_request_from_client_and_respond(i, prot) < 0) {
                        close(i);
                        FD_CLR (i, &active_fd_set);
                    }
                }
            }
        }
    }
}





int
read_request_from_client_and_respond(int filedes, Protocol &prot)
{
    int diavasa=0;

    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    bool flagLOG_ON     = false;
    bool flagGET_CLIENTS= false;
    bool flagLOG_OFF    = false;

    while(read(filedes, buf, 1) > 0) {  /* Receive 1 char */
//        printf("diavasa %d bytes\n", ++diavasa);

        instruction += buf;
//        cout << instruction<<endl;

        if (instruction == "LOG_ON") {
            flagLOG_ON = true;
            break;
        }

        if (instruction == "GET_CLIENTS") {
            flagGET_CLIENTS = true;
            break;
        }

        if (instruction == "LOG_OFF") {
            flagLOG_OFF = true;
            break;
        }


    }


    clientsTuple tupl;

    if (flagLOG_ON) {
        prot.recv_LOG_ON(filedes , tupl);
        prot.broadcast_USER_ON(tupl);

    }
    if (flagGET_CLIENTS) {
        //https://piazza.com/class/js1lfr8hs4jj7?cid=273

        prot.recv_GET_CLIENTS(filedes, tupl);
        /*now add the new client to the list*/
        usleep(1);
        prot.send_CLIENTS_LIST(tupl);
        prot.add_newClient(tupl);


        cout <<">Clients_list after LOG_ON & GET_CLIENTS: \t";
        cout << prot.clients_list<<endl;


    }
    if (flagLOG_OFF) {
        prot.recv_LOG_OFF(filedes, tupl);
        /* first remove the client from list to not broadcast to it the USER_OFF */
        if (! prot.remove_client(tupl) ) //client was deleted successfully from the lsit
            prot.broadcast_USER_OFF(tupl); //todo CHANGE IT TO SEND ERROR_IP_PORT_NOT_FOUND_IN_LIST
        else //client wasnt found in the list
            cout << "ERROR_IP_PORT_NOT_FOUND_IN_LIST" <<endl;

        cout <<">Clients_list after LOG_OFF: \t";
        cout << prot.clients_list<<endl;

    }



    return -1;

}








/* Wait for all dead child processes */
void sigchld_handler (int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
