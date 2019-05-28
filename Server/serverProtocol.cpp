//
// Created by dimitrisgan on 25/5/2019.
//

#include "serverProtocol.h"


void start_session(myString buffer2send){

    cout << "anoigw session-socket gia na milhsw ston client\n";

}


myString convertBinaryIpToString(uint32_t ipB){
    myString ipStr;

    uint32_t  afterCastIp = ntohl(ipB);

//    struct sockaddr_in name;
    struct sockaddr_in sa;

    char str[INET_ADDRSTRLEN];

    sa.sin_addr.s_addr = afterCastIp;

    // now get it back and print it
    inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
    myString clientIp(str);

    return ipStr;
}

//======================================================

Protocol::Protocol(ArgumentsKeeper args) : args(args) {}


int Protocol::recv_LOG_ON(int filedes, clientsTuple &tupl) {

//    if (read(sock,logOn.getMyStr(), logOn.size()) < 0)
//        perror_exit("write in LOG ON");
//
    uint32_t ipAddr;
    if (read(filedes, &ipAddr, sizeof(uint32_t)) < 0) //todo needs while () defensive programming
        perror_exit("read Ip in LOG ON");

    uint16_t  readClientsPort;
    if (read(filedes, &readClientsPort, sizeof(uint16_t)) < 0) //todo needs while () defensive programming
        perror_exit("read port in LOG ON");


    uint32_t retIp;
    uint16_t retPort;
    retIp = ntohl(ipAddr);
    retPort = ntohs(readClientsPort);

    tupl.ip     = retIp;
    tupl.port   = retPort;


    cout << "afterCastClientsIp = "<<retIp<<endl;
    cout << "afterCastClientsPort = "<<retPort<<endl;




////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//todo na fugei olo to apo katw
////////////////////////////////////////////////////////////////
//
//    myString hi("GET_CLIENTS");
//
//    struct sockaddr_in client;
//    struct sockaddr_in sa;
//
//    char str[INET_ADDRSTRLEN];
//
//    sa.sin_addr.s_addr = afterCastIp;
//
//    // now get it back and print it
//    inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
//    myString clientIp(str);
//
//
//    printf("MAN MOU TO IP SE STRING TOU FILOU CLIENT MAS EINIA: %s ",clientIp.getMyStr());
//    printf(" KAI TO PORT : %d \n",afterCastClientsPort);
//
//    // store this IP address in sa:
////    inet_pton(AF_INET, clientIp.getMyStr(), &(sa.sin_addr));
//
//
//    int     sock2write2client;
//    /* Create socket */
//    if ((sock2write2client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//        perror_exit("socket");
//
//    /* Connect to the client. */
//    init_sockaddr(&client, clientIp.getMyStr() , afterCastClientsPort);
//
//
//    if (0 > connect (sock2write2client, (struct sockaddr *) &client, sizeof (client)))
//    {
//        perror ("connect (client)");
//        exit (EXIT_FAILURE);
//    }
//
//
//    if (write(sock2write2client,hi.getMyStr(), hi.size()) < 0)
//        perror_exit("write LOG ON");
//
//
//    close(sock2write2client);
//
//    //////////////////////////////////////////////////



//    this->broadcast_USER_ON(newsock,tupl);
    return 0;
}


int Protocol::add_newClient_tupl(const clientsTuple & tupl) {

    /*save them to the list of tuples if they dont already exist*/

    if (! this->connectedClients_list.exists(tupl))
        this->connectedClients_list.insert_last(tupl);


    return 0;
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


//////////////////////////////////////////////////






int Protocol::broadcast_USER_ON(const clientsTuple &tupl) {

    myString userOn("USER_ON");

    myString ipStr;

    for (auto &item : this->connectedClients_list) {

        if (item == tupl)
            continue;

        ipStr = convertBinaryIpToString(item.ip);

        printf("MAN MOU TO IP SE STRING TOU FILOU CLIENT MAS EINIA: %s ",ipStr.getMyStr());
        printf(" KAI TO PORT : %d \n",item.port);

        int newsock = create_socket_and_connect(ipStr,item.port);

        if (write(newsock , userOn.getMyStr() , userOn.size()) < 0)
            perror_exit("write USER_ON");

        uint32_t ipToSend = htonl(item.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP");

        uint16_t portToSend = htons(item.port);
        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT");

        close(newsock);
    }

    return 0;
}

int Protocol::recv_GET_CLIENTS() {

    send_CLIENTS_LIST();

    return 0;
}

int Protocol::send_CLIENTS_LIST() {
    return 0;
}

int Protocol::broadcast_USER_OFF(int newsock, clientsTuple tupl) {
    //TODO KANEI BROADCAST PANTOU KAI O CLIENT POU VLEPEI OTI AFORA TON IDIO APLA TO AGNOEI
    //TODO OPOTE STELNW KAI TO <IP,PORT>

    return 0;
}



//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////



