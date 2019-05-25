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


void perror_exit(char *message);

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;

    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    argmKeeper.printArgs();

    //todo connectToDropBox_server() //[LOG ON]

//    int sock;
//    if(( sock = socket ( AF_INET , SOCK_STREAM ,0) ) ==-1)
//            perror ( " Socket creation failed ! " ) ;

    Protocol prot(argmKeeper);

    int             port, sock, i;
    char            buf[256];

    struct sockaddr_in server;
    struct sockaddr *serverptr = (struct sockaddr*)&server;
    struct hostent *rem;

//    if (argc != 3) {
//        printf("Please give host name and port number\n");
//        exit(1);}
    /* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");
    /* Find server address */
/*
    if ((rem = gethostbyname(argv[1])) == NULL) {
*/
    if ((rem = gethostbyname(argmKeeper.serverIp.getMyStr())) == NULL) {

        herror("gethostbyname"); exit(1);
    }
//    port = atoi(argv[2]); /*Convert port number to integer*/

    port = argmKeeper.serverPort.to_int();
    server.sin_family = AF_INET;       /* Internet domain */
    memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
    server.sin_port = htons(port);         /* Server port */
    /* Initiate connection */
    if (connect(sock, serverptr, sizeof(server)) < 0)
        perror_exit("connect");
    printf("Connecting to %s port %d\n",argmKeeper.serverIp.getMyStr() , port);

    //todo send_GET_CLIENTS_to_server()

    prot.send_LOG_ON(sock);
//    do {
//        printf("Give input string: ");
//        fgets(buf, sizeof(buf), stdin);	/* Read from stdin*/
//        for(i=0; buf[i] != '\0'; i++) { /* For every char */
//            /* Send i-th character */
//            if (write(sock, buf + i, 1) < 0)
//                perror_exit("write");
//            /* receive i-th character transformed */
//            if (read(sock, buf + i, 1) < 0)
//                perror_exit("read");
//        }
//        printf("Received string: %s", buf);
//    } while (strcmp(buf, "END\n") != 0); /* Finish on "end" */

//    close(sock);                 /* Close socket and exit */


    return 0;
}


//int bind_on_port ( int sock , short port ) {
//    struct sockaddr_in server ;
//    server . sin_family = AF_INET ;
//    server . sin_addr.s_addr = htonl ( INADDR_ANY ) ;
//    server . sin_port = htons (port) ;
//    return bind ( sock , ( struct sockaddr *) & server ,sizeof ( server ) );
//}
