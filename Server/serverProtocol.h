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

    int recv_header(int filedes,clientsTuple &tupl); //header = ip,port

public:

    linkedList <clientsTuple> clients_list;

    explicit Protocol( ArgumentsKeeper args);

    int recv_LOG_ON(int filedes, clientsTuple & tupl);

    int recv_GET_CLIENTS(int filedes, clientsTuple &tupl);

    int send_CLIENTS_LIST(const clientsTuple & tupl);

    int recv_LOG_OFF(int filedes, clientsTuple &tupl);

    int add_newClient(const clientsTuple &tupl);

    int broadcast_USER_ON(const clientsTuple &tupl); /*tupl = announce the new client*/

    int broadcast_USER_OFF(clientsTuple &tupl); /*tupl = announce client that is gonna quit*/

    int remove_client(clientsTuple &tupl);


};





#endif //SERVER_SERVERPROTOCOL_H
