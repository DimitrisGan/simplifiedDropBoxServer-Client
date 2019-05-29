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


int
read_from_others_requests_and_respond(int filedes, Protocol &prot);


int main(int argc, char **argv) {


    ArgumentsKeeper argmKeeper;
    argmParser(argc, argv , argmKeeper);

    argmKeeper.printArgs();

    //todo connectToDropBox_server() //[LOG ON]

//    int sock_ToWriteToServer;
//    if(( sock_ToWriteToServer = socket ( AF_INET , SOCK_STREAM ,0) ) ==-1)
//            perror ( " Socket creation failed ! " ) ;

    Protocol prot(argmKeeper);





    int     i;

    uint16_t serverPort,listenPort;
    serverPort = static_cast<uint16_t>(argmKeeper.serverPort.to_int());
    listenPort = static_cast<uint16_t>(argmKeeper.portNum.to_int());


    int sock_writes_to_server_LOG_ON = create_socket_and_connect(argmKeeper.serverIp,serverPort);

    prot.send_LOG_ON(sock_writes_to_server_LOG_ON);

    int sock_writes_to_server_GET_CLIENTS = create_socket_and_connect(argmKeeper.serverIp,serverPort);

    prot.send_GET_CLIENTS(sock_writes_to_server_GET_CLIENTS);





    /*======    FROM HERE MAKE A NEW SOCKET FOR LISTENING   ======*/

    int sock_to_listen = make_socket_and_bind(listenPort);


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
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
            perror_exit("select");
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
                       perror_exit("accept");

                    fprintf (stderr,
                             "Other: connect from host %s, port %d.\n",
                             inet_ntoa (other.sin_addr),
                             ntohs (other.sin_port));
                    FD_SET (newsock, &active_fd_set);
                }
                else
                {
                    /* Data arriving on an already-connected socket. */
                    if (read_from_others_requests_and_respond(i, prot) < 0)
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



int
read_from_others_requests_and_respond(int filedes, Protocol &prot)
{
    int diavasa=0;



    cout << "EKANA CONNECT APO SERVER!\n";

    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    bool flagUSER_ON     = false;
    bool flagCLIENT_LIST= false;
//    bool flagLOG_OFF    = false;

    while(read(filedes, buf, 1) > 0) {  /* Receive 1 char */
//        printf("diavasa %d bytes\n", ++diavasa);

        instruction += buf;

//        cout << "To instruction exei timh = " << instruction << endl;

        if (instruction == "USER_ON") {
            flagUSER_ON = true;
            break;
        }

        if (instruction == "CLIENT_LIST") {
            flagCLIENT_LIST = true;
            break;
        }
//
//        if (instruction == "LOG_OFF") {
//            flagLOG_OFF = true;
//            cout << instruction;
//            break;
//        }


    }

    cout << "INFO::Instruction read = " << instruction << endl;



    //todo SOOSSSS!!!TO LOG_ON KAI OLA TA ALLA MESSAGES THA TA STELNW SE ALLA SOCKETS
    //TODO SAYTA POU KATHE PROCESS KANIE LISTEN KAI DEXETAI CONNECTION
    //TODO ARA PREPEI NA KANW ESTABLISH KAINOURGIO CONNECTION OPOU SERVER GINETAI CLIENT
    //TODO KAI CLIENT-SERVER

     if (flagUSER_ON) {
         clientsTuple newClient;
         prot.recv_USER_ON(filedes,newClient);
         prot.add_newClient(newClient);

         cout <<"Printing the list after USER_ON: \t";
         cout << prot.client_list<<endl;


     }
     if (flagCLIENT_LIST){
         linkedList <clientsTuple> existingClients_list;
         prot.recv_CLIENTS_LIST(filedes ,existingClients_list );
         prot.add_list_of_existing_clients(existingClients_list);

         cout <<"Printing the list after CLIENT_LIST: \t";
         cout << prot.client_list<<endl;


     }


    return -1;

}
