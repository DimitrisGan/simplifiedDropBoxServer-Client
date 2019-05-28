//
// Created by dimitrisgan on 21/5/2019.
//

#ifndef CLIENT_CLIENTPROTOCOL_H
#define CLIENT_CLIENTPROTOCOL_H


#include "myString.h"
#include "mylinkedList.h"

#include "assistantFunctions.h"

#include <stdio.h>
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <unistd.h>          /* read, write, close */
#include <netdb.h>	         /* gethostbyaddr */
#include <stdlib.h>	         /* exit */
#include <string.h>	         /* strlen */
#include <arpa/inet.h>
#include <bitset>
#include <stdint.h>

#include "clientTuple.h"

class Protocol{
    ArgumentsKeeper args;
//    int sock;

    linkedList<clientsTuple> tupl_list;

    myString end;

public:
    explicit Protocol(ArgumentsKeeper args);

    int send_LOG_ON(int sock);

    int send_GET_CLIENTS(int sock);

    int send_LOG_OFF(int sock);


    int recv_CLIENTS_LIST();


};


#endif //CLIENT_CLIENTPROTOCOL_H
