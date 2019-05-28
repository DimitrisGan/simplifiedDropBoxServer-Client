#include <iostream>

//#include <sys/socket.h>             /* sockets */
//#include <sys/types.h>              /* sockets */
//#include <netinet/in.h>             /* Internet sockets */
//#include <arpa/inet.h>              /* for hton */


#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */


#include "assistantFunctions.h"
#include "socketManipulation.h"
#include "clientProtocol.h"


int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;

    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    argmKeeper.printArgs();

    //todo connectToDropBox_server() //[LOG ON]

//    int sock_ToWriteToServer;
//    if(( sock_ToWriteToServer = socket ( AF_INET , SOCK_STREAM ,0) ) ==-1)
//            perror ( " Socket creation failed ! " ) ;

    Protocol prot(argmKeeper);





    int     sock_writes_to_server, i;

    uint16_t serverPort,listenPort;
    serverPort = static_cast<uint16_t>(argmKeeper.serverPort.to_int());
    listenPort = static_cast<uint16_t>(argmKeeper.portNum.to_int());

    struct sockaddr_in server;
    struct sockaddr_in servername;



    /* Create socket */
    if ((sock_writes_to_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

    /* Connect to the server. */
    init_sockaddr(&servername, argmKeeper.serverIp.getMyStr(), serverPort);

    if (0 > connect (sock_writes_to_server,(struct sockaddr *) &servername,sizeof (servername)))
    {
        perror ("connect (client)");
        exit (EXIT_FAILURE);
    }




    //todo send_GET_CLIENTS_to_server()

    prot.send_LOG_ON(sock_writes_to_server);

    /////////////////////////////////////////////////////////////////
    /*FROM HERE MAKE THE SOCKET TO LISTEN*/

    int sock_to_listen;
    sock_to_listen = make_socket_and_bind(listenPort);



    /* Listen for connections */
    if (listen(sock_to_listen, 5) < 0) perror_exit("listen");
    printf("Listening for connections to port %d\n", listenPort);



    fd_set active_fd_set, read_fd_set;
    struct sockaddr_in other;
    socklen_t size;

    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (sock_to_listen, &active_fd_set);

    while (true)
    {
        cout << "hi I am Client\n";
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror ("select");
            exit (EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET (i, &read_fd_set))
            {
                if (i == sock_to_listen)
                {
                    /* Connection request on original socket. */
                    int newsock;
                    size = sizeof (other);
                    newsock = accept (sock_to_listen, (struct sockaddr *) &other, &size);
                    if (newsock < 0)
                    {
                        perror ("accept");
                        exit (EXIT_FAILURE);
                    }
                    fprintf (stderr,
                             "Other: connect from host %s, port %hd.\n",
                             inet_ntoa (other.sin_addr),
                             ntohs (other.sin_port));
                    FD_SET (newsock, &active_fd_set);
                }
                else
                {
                    /* Data arriving on an already-connected socket. */
                    if (read_from_others (i,prot) < 0)
                    {
                        close (i);
                        FD_CLR (i, &active_fd_set);
                    }
                }
            }
    }



    return 0;
}


//int bind_on_port ( int sock , short port ) {
//    struct sockaddr_in server ;
//    server . sin_family = AF_INET ;
//    server . sin_addr.s_addr = htonl ( INADDR_ANY ) ;
//    server . sin_port = htons (port) ;
//    return bind ( sock , ( struct sockaddr *) & server ,sizeof ( server ) );
//}
