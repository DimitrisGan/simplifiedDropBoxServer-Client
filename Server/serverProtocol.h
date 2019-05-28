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

#include "mylinkedList.h"
#include "myString.h"
#include "assistantFunctions.h"
#include "socketManipulation.h"
#include "clientTuple.h"


class Protocol{
    ArgumentsKeeper args;

    linkedList <clientsTuple> connectedClients_list;

//    int sock;

//    linkedList<myTuple> tupl_list;



    int broadcast_USER_OFF(int newsock,clientsTuple tupl);

public:
    explicit Protocol( ArgumentsKeeper args);

    int recv_LOG_ON(int filedes, clientsTuple & tupl);

    int recv_GET_CLIENTS();

    int recv_LOG_OFF(int filedes);

    int add_newClient_tupl(const clientsTuple & tupl);

    int broadcast_USER_ON(const clientsTuple &newClientToAnnounce);

    int send_CLIENTS_LIST();


};





#endif //SERVER_SERVERPROTOCOL_H
