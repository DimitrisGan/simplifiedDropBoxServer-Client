//
// Created by dimitrisgan on 28/5/2019.
//

#include "socketManipulation.h"


myString convertBinaryIpToString(uint32_t ipB){
    myString ipStr;

//    uint32_t  afterCastIp = ntohl(ipB);

//    struct sockaddr_in name;
    struct sockaddr_in sa;

    char str[INET_ADDRSTRLEN];

    sa.sin_addr.s_addr = ipB;

    // now get it back and print it
    inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);

    ipStr = str;
    return ipStr;
}



int
make_socket_and_bind(uint16_t port)
{
    int sock;
    struct sockaddr_in name;

    /* Create the socket. */
    sock = socket (PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    }

    /* Give the socket a name. */
    name.sin_family = AF_INET;  /* Internet domain */
    name.sin_port = htons (port);   /* The given port */
    name.sin_addr.s_addr = htonl (INADDR_ANY);
    /* Bind socket to address */
    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    return sock;
}

int create_socket_and_connect(myString ip,uint16_t port){

    int     sock;

    struct sockaddr_in client;

/* Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

/* Connect to the client. */
//cout <<"port ==== "<<port<<endl;
    init_sockaddr(&client, ip.getMyStr() , port);


    if (0 > connect (sock, (struct sockaddr *) &client, sizeof (client)))
    {
        perror ("connect (client)");
        exit (EXIT_FAILURE);
    }

    return sock;
}



void
init_sockaddr (struct sockaddr_in *name,
               const char *hostname,
               uint16_t port)
{
    struct hostent *hostinfo;



    name->sin_family = AF_INET;
    name->sin_port = htons (port);
//    name->sin_port = port;
    hostinfo = gethostbyname (hostname);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", hostname);
        exit (EXIT_FAILURE);
    }
    name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}



