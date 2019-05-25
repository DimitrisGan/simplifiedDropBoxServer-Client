//
// Created by dimitrisgan on 21/5/2019.
//

#ifndef CLIENT_CLIENTPROTOCOL_H
#define CLIENT_CLIENTPROTOCOL_H


#include "myString.h"
#include "assistantFunctions.h"
#include "tuples.h"

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


class Protocol{
    ArgumentsKeeper args;
//    int sock;

    linkedList<myTuple> tupl_list;

    myString end;

public:
    explicit Protocol(ArgumentsKeeper args);

    int send_LOG_ON(int sock);

    int send_GET_CLIENTS(int sock);


    int recv_CLIENTS_LIST();


};

int Protocol::send_LOG_ON(int sock) {
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent  *mymachine;
    int hostname;

    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    cout << "hostname = "<< hostbuffer<<endl;
    checkHostName(hostname);

    // To retrieve host information
    mymachine = gethostbyname(hostbuffer);
    checkHostEntry(mymachine);


    // To convert an Internet network
    // address into ASCII string
    /*
     * IPbuffer = inet_ntoa(*((struct in_addr*)
            mymachine->h_addr_list[0]));


    printf("Hostname: %s\n", hostbuffer);
    printf("Host IP: %s\n", IPbuffer);

     */

    /*char hostname [50]*/ char symbolicip [50];
    struct in_addr **addr_list = nullptr;

    if ( (mymachine = gethostbyname ( hostbuffer )) == NULL ){
        printf(" Could not resolved Name : %s \n ",hostbuffer);
    }
    else{


        printf ( " Name To Be Resolved : %s \n " , mymachine -> h_name ) ;
        printf ( " Name Length in Bytes : % d \n " , mymachine -> h_length ) ;
        addr_list = ( struct in_addr **) mymachine -> h_addr_list ;
        for ( int i = 0; addr_list [ i ] != NULL ; i ++) {
            strcpy ( symbolicip , inet_ntoa (* addr_list [ i ]) ) ;
            printf ( " %s resolved to %s \n " , mymachine -> h_name , symbolicip ) ;
        }
    }
    cout << " edw koita megale :"<<( struct in_addr **) mymachine -> h_addr_list[0]<<endl;


    myString logOn("LOG_ON");

    myString instruction2send = logOn;
    instruction2send+= " ";
//    instruction2send+= zip_it(this->args.)



    if (write(sock,logOn.getMyStr(), logOn.size()) < 0)
        perror_exit("write in LOG ON");

    uint32_t ipInbinary = htonl(addr_list[0]->s_addr);

    cout << " EDWWWW :"<<ipInbinary<<endl;

    print_ip(addr_list[0]->s_addr);

    if (write(sock, &ipInbinary , sizeof(uint32_t)) < 0)
        perror_exit("write in IP");


    uint32_t portBinary = htonl(static_cast<uint32_t>(this->args.portNum.to_int()));

    if (write(sock, &portBinary , sizeof(uint32_t)) < 0)
        perror_exit("write in PORT");


    close(sock);
    return 0;
}

Protocol::Protocol(ArgumentsKeeper args) : args(args) {}

int Protocol::send_GET_CLIENTS(int sock) {
    return 0;
}

#endif //CLIENT_CLIENTPROTOCOL_H
