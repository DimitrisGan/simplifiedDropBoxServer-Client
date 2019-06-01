//
// Created by dimitrisgan on 1/6/2019.
//

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
#include "criticalSection.h"
int main(int argc, char **argv) {


    int i;

    uint16_t serverPort, listenPort;

    thread_protocol thr;

    myString ip("127.0.1.1");
    int sock_writes_FILE_LIST_and_recv_FILE_LIST= create_socket_and_connect(ip, 8000);



    thr.send_GET_FILE_LIST_and_recv_FILE_LIST(  sock_writes_FILE_LIST_and_recv_FILE_LIST    );

    return 0;
}