//
// Created by dimitrisgan on 28/5/2019.
//

#include "socketManipulation.h"



uint32_t convertStringIpToBinary(myString ip){

    struct sockaddr_in sa;

    int s = inet_pton(AF_INET,ip.getMyStr() ,& sa.sin_addr );
    if (s <= 0) {
        if (s == 0)
            fprintf(stderr, "Not in presentation format");
        else
            perror_exit("inet_pton");
    }


    uint32_t ipInbinary = htonl(sa.sin_addr.s_addr );

    return ipInbinary;
}

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


    int enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror_exit("setsockopt(SO_REUSEADDR) failed");

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
    hostinfo = gethostbyname (hostname);
    if (hostinfo == NULL)
    {
        fprintf (stderr, "Unknown host %s.\n", hostname);
        exit (EXIT_FAILURE);
    }

    name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}

myString getMyIpInStr() {
    myString ip;

    char hostbuffer[256];
    char *IPbuffer;
    struct hostent  *mymachine;
    int hostname;

    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);

    // To retrieve host information
    mymachine = gethostbyname(hostbuffer);
    checkHostEntry(mymachine);

    // To convert an Internet network
    // address into ASCII string
    IPbuffer = inet_ntoa(*((struct in_addr*)
            mymachine->h_addr_list[0]));


    ip = IPbuffer;
    return ip;
}

myString convertBInaryPortToString(uint16_t portB) {

    char str[500];
    sprintf(str, "%u", portB);
    myString strPort(str);

    return strPort;
}




