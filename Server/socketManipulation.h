//
// Created by dimitrisgan on 28/5/2019.
//

#ifndef SERVER_SOCKETMANIPULATION_H
#define SERVER_SOCKETMANIPULATION_H


#include <stdio.h>
#include <sys/wait.h>	     /* sockets */
#include <sys/types.h>	     /* sockets */
#include <sys/socket.h>	     /* sockets */
#include <netinet/in.h>	     /* internet sockets */
#include <netdb.h>	         /* gethostbyaddr */
#include <unistd.h>	         /* fork */
#include <stdlib.h>	         /* exit */
#include <ctype.h>	         /* toupper */
#include <signal.h>          /* signal */
#include <arpa/inet.h>
#include "myString.h"


myString
convertBinaryIpToString(uint32_t ipB);


int
make_socket_and_bind(uint16_t port);

void
init_sockaddr (struct sockaddr_in *name,
               const char *hostname,
               uint16_t port);





#endif //SERVER_SOCKETMANIPULATION_H
