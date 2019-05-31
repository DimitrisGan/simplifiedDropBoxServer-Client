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


    int send_header(int sock); //header = ip,port

    int recv_header(int filedes,clientsTuple &tupl); //header = ip,port


public:

    linkedList<clientsTuple> clients_list; //todo tha ginei global gia na einai shared gia ola ta threads!!!


    explicit Protocol(ArgumentsKeeper args);

    /*Request and responds from/to server */
    int send_LOG_ON(int sock);
    int recv_USER_ON(int sock , clientsTuple &tupl); /*tupl = newClient*/
    int add_client(const clientsTuple &tupl);


    int send_GET_CLIENTS(int sock);
    int recv_CLIENTS_LIST(int sock , linkedList <clientsTuple> &existingClients_list);
    int add_list_of_existing_clients(linkedList <clientsTuple> &existingClients_list);


    int send_LOG_OFF(int sock);
    int recv_USER_OFF(int sock , clientsTuple &tupl); /*clients that quits*/
    int remove_client(const clientsTuple &tupl);


    /*Request and responds from/to worker threads from others clients */
    int recv_GET_FILE_LIST(int sock);
    int respond_with_FILE_LIST(int sock);




};


#endif //CLIENT_CLIENTPROTOCOL_H
