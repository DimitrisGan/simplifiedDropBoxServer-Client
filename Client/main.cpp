#include <iostream>
#include "assistantFunctions.h"
#include "clientProtocol.h"


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


int diavasa=0;

void perror_exit(char *message);

int
make_socket (uint16_t port)
{
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;  /* Internet domain */
    name.sin_port = htons (port);   /* The given port */
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    /* Bind socket to address */
    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    return sock;
}

void
init_sockaddr (struct sockaddr_in *name,
               const char *hostname,
               uint16_t port)
{
    struct hostent *hostinfo;

    name->sin_family = AF_INET;
    name->sin_port = htons (port);
    hostinfo = gethostbyname (hostname);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", hostname);
        exit (EXIT_FAILURE);
    }
    name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}


int
read_from_others (int filedes ,Protocol &prot )
{
//    char buffer[MAXMSG];
//    int nbytes;
//    nbytes = read (filedes, buffer, MAXMSG);
//    if (nbytes < 0)
//    {
//        /* Read error. */
//        perror ("read");
//        exit (EXIT_FAILURE);
//    }
//    else if (nbytes == 0)
//        /* End-of-file. */
//        return -1;
//    else
//    {
//        /* Data read. */
//        fprintf (stderr, "Server: got message: `%s'\n", buffer);
//        return 0;
//    }

    cout << "EKANA CONNECT APO SERVER!\n";

    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    bool flagLOG_ON     = false;
    bool flagGET_CLIENTS= false;
    bool flagLOG_OFF    = false;

    while(read(filedes, buf, 1) > 0) {  /* Receive 1 char */
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


    //todo SOOSSSS!!!TO LOG_ON KAI OLA TA ALLA MESSAGES THA TA STELNW SE ALLA SOCKETS
    //TODO SAYTA POU KATHE PROCESS KANIE LISTEN KAI DEXETAI CONNECTION
    //TODO ARA PREPEI NA KANW ESTABLISH KAINOURGIO CONNECTION OPOU SERVER GINETAI CLIENT
    //TODO KAI CLIENT-SERVER

    /* if (flagLOG_ON) {
         prot.recv_LOG_ON(newsock);
 //        prot.broadcast_USER_ON();

     }

 */

    return 0;

}


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

    char            buf[256];

    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

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



//    /* Find server address */
///*
//    if ((rem = gethostbyname(argv[1])) == NULL) {
//*/
//    if ((rem = gethostbyname(argmKeeper.serverIp.getMyStr())) == NULL) {
//
//        herror("gethostbyname"); exit(1);
//    }
//
//    port = argmKeeper.serverPort.to_int();
//    server.sin_family = AF_INET;       /* Internet domain */
//    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
//    server.sin_port = htons(port);         /* Server port */

    //todo send_GET_CLIENTS_to_server()

    prot.send_LOG_ON(sock_writes_to_server);


    /* Initiate connection */
//    if (connect(sock_ToWriteToServer, serverptr, sizeof(server)) < 0)
//        perror_exit("connect");
//        printf("Connecting to %s port %d\n",argmKeeper.serverIp.getMyStr() , port);
//





//    do {
//        printf("Give input string: ");
//        fgets(buf, sizeof(buf), stdin);	/* Read from stdin*/
//        for(i=0; buf[i] != '\0'; i++) { /* For every char */
//            /* Send i-th character */
//            if (write(sock_ToWriteToServer, buf + i, 1) < 0)
//                perror_exit("write");
//            /* receive i-th character transformed */
//            if (read(sock_ToWriteToServer, buf + i, 1) < 0)
//                perror_exit("read");
//        }
//        printf("Received string: %s", buf);
//    } while (strcmp(buf, "END\n") != 0); /* Finish on "end" */

    close(sock_writes_to_server);                 /* Close socket and exit */


    /////////////////////////////////////////////////////////////////
    /*FROM HERE MAKE THE SOCKET TO LISTEN*/

    int sock_to_listen;
    sock_to_listen = make_socket(listenPort);



    /* Listen for connections */
    if (listen(sock_to_listen, 5) < 0) perror_exit("listen");
    printf("Listening for connections to port %d\n", listenPort);



    fd_set active_fd_set, read_fd_set;
    struct sockaddr_in clientname;
    socklen_t size;

    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (listenPort, &active_fd_set);

    while (true)
    {
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
                    size = sizeof (clientname);
                    newsock = accept (sock_to_listen, (struct sockaddr *) &clientname, &size);
                    if (newsock < 0)
                    {
                        perror ("accept");
                        exit (EXIT_FAILURE);
                    }
                    fprintf (stderr,
                             "Server: connect from host %s, port %hd.\n",
                             inet_ntoa (clientname.sin_addr),
                             ntohs (clientname.sin_port));
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
