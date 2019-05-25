//
// Created by dimitrisgan on 21/5/2019.
//

#ifndef SERVER_SERVERPROTOCOL_H
#define SERVER_SERVERPROTOCOL_H


#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */


#include "myString.h"
#include "assistantFunctions.h"
//#include "tuples.h"


class Protocol{
    ArgumentsKeeper args;
//    int sock;

//    linkedList<myTuple> tupl_list;

public:
    explicit Protocol( ArgumentsKeeper args);

    int recv_LOG_ON(int newsock);

    int recv_GET_CLIENTS();


    int recv_CLIENTS_LIST();


};

int Protocol::recv_LOG_ON(int newsock) {
    myString logOn("LOG_ON");

//    if (read(sock,logOn.getMyStr(), logOn.size()) < 0)
//        perror_exit("write in LOG ON");
//
    uint32_t ipAddr;

    read(newsock, &ipAddr, sizeof(uint32_t)) ; //todo needs while () defensive programming

    uint32_t  afterCastIp = ntohl(ipAddr);

    cout <<" "<<afterCastIp;

    uint32_t  newClientsPort;
    read(newsock, &newClientsPort, sizeof(uint32_t)) ; //todo needs while () defensive programming

    uint32_t  afterCastnewClientsPort = ntohl(newClientsPort);


    cout << "newClientsPort = "<<newClientsPort<<endl;
    cout << "afterCastnewClientsPort = "<<afterCastnewClientsPort<<endl;


//    	putchar(buf[0]);           /* Print received char */
//    	/* Capitalize character */
//    	buf[0] = toupper(buf[0]);
//    	/* Reply */
//    	if (write(newsock, buf, 1) < 0)
//    	    perror_exit("write");

    printf("Closing connection.\n");
    close(newsock);	  /* Close socket */


    print_ip(afterCastIp);


    return 0;
}

Protocol::Protocol(ArgumentsKeeper args) : args(args) {}



#endif //SERVER_SERVERPROTOCOL_H
