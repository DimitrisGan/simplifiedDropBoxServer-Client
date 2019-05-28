//
// Created by dimitrisgan on 25/5/2019.
//

#include "serverProtocol.h"


Protocol::Protocol(ArgumentsKeeper args) : args(args) {}


int Protocol::recv_LOG_ON(int newsock /*, uint32_t &retClientsIp, uint32_t &retClientsPort*/) {
    myString logOn("LOG_ON");

//    if (read(sock,logOn.getMyStr(), logOn.size()) < 0)
//        perror_exit("write in LOG ON");
//
    uint32_t ipAddr;

    read(newsock, &ipAddr, sizeof(uint32_t)) ; //todo needs while () defensive programming

    uint32_t  afterCastIp = ntohl(ipAddr);

    cout <<" "<<afterCastIp;

    uint16_t  newClientsPort;
    read(newsock, &newClientsPort, sizeof(uint16_t)) ; //todo needs while () defensive programming

    uint16_t  afterCastnewClientsPort = ntohs(newClientsPort);

    cout << "afterCastnewClientsPort = "<<afterCastnewClientsPort<<endl;


    print_ip(afterCastIp);

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//todo na fugei olo to apo katw
////////////////////////////////////////////////////////////////

    myString hi("GET_CLIENTS");

    struct sockaddr_in name;

//    init_sockaddr(&name, ipOfClient, afterCastnewClientsPort);
//    init_sockaddr(&name, "127.0.1.1", afterCastnewClientsPort);


//    char saddr[INET_ADDRSTRLEN];
    struct sockaddr_in sa;
    char str[INET_ADDRSTRLEN];
    sa.sin_addr.s_addr = afterCastIp;

    // now get it back and print it
    inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
    myString clientIp(str);


    printf("MAN MOU TO IP SE STRING TOU FILOU CLIENT MAS EINIA: %s ",clientIp.getMyStr());
    printf(" KAI TO PORT : %d \n",afterCastnewClientsPort);

    // store this IP address in sa:
//    inet_pton(AF_INET, clientIp.getMyStr(), &(sa.sin_addr));



//    int sock2respondLOG_ON;
//    struct sockaddr_in clientName;
//
//    /* Create socket */
//    if ((sock2respondLOG_ON = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//        perror_exit("socket");
//
//    /* Connect to the server. */
//    init_sockaddr(&clientName, clientIp, clientPort);
//
//
//    if (0 > connect (sock2respondLOG_ON,(struct sockaddr *) &clientName,sizeof (clientName)))
//    {
//        perror ("connect (client)");
//        exit (EXIT_FAILURE);
//    }

    cout << " GRAFW MHNUMA STON CLIENT !\n";

    int     sock2write2client;
    /* Create socket */
    if ((sock2write2client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror_exit("socket");

    /* Connect to the server. */
    init_sockaddr(&name, clientIp.getMyStr() , afterCastnewClientsPort);


    if (0 > connect (sock2write2client, (struct sockaddr *) &name, sizeof (name)))
    {
        perror ("connect (client)");
        exit (EXIT_FAILURE);
    }

    cout << "eftasa edw kai einai katorthwma!!\n";

    if (write(sock2write2client,hi.getMyStr(), hi.size()) < 0)
        perror_exit("write LOG ON");


    close(sock2write2client);

    clientsTuple tupl(afterCastIp,afterCastnewClientsPort);

    if (! this->connectedClients_list.exists(tupl))
        this->connectedClients_list.insert_last(tupl);



//    this->broadcast_USER_ON(newsock,tupl);
    return 0;
}










int Protocol::broadcast_USER_ON(int newsock,clientsTuple tupl) {


    myString userOn("USER_ON");

    for (auto &item : this->connectedClients_list) {

        if (item == tupl)
            continue;

        //todo edw kanw to broadcast
        if (write(newsock , userOn.getMyStr() , userOn.size()) < 0)
            perror_exit("write USER_ON");

        uint32_t ipToSend = htonl(item.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP");

        uint16_t portToSend = htons(item.port);
        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT");

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



int
read_from_client (int filedes ,Protocol &prot )
{
    int diavasa=0;

    char buf[1];
    myString whitespace(" ");
    myString instruction("");

    bool flagLOG_ON     = false;
    bool flagGET_CLIENTS= false;
    bool flagLOG_OFF    = false;

    while(read(filedes, buf, 1) > 0) {  /* Receive 1 char */
        printf("diavasa %d bytes\n", ++diavasa);

        instruction += buf;

        cout << "To instruction exei timh = " << instruction << endl;
        if (instruction == "LOG_ON") {
            flagLOG_ON = true;
            cout << instruction;
            break;
        }

        if (instruction == "GET_CLIENTS") {
            flagGET_CLIENTS = true;
            cout << instruction;
            break;
        }

        if (instruction == "LOG_OFF") {
            flagLOG_OFF = true;
            cout << instruction;
            break;
        }


    }

    uint32_t  clientIp;
    uint16_t  clientPort;
    if (flagLOG_ON) {
        prot.recv_LOG_ON(filedes /*, clientIp, clientPort*/);
//        prot.broadcast_USER_ON();
    }
    if (flagGET_CLIENTS) {
//        prot.recv_LOG_ON(filedes /*, clientIp, clientPort*/);
//        prot.broadcast_USER_ON();
    }
    if (flagLOG_OFF) {
//        prot.recv_LOG_ON(filedes /*, clientIp, clientPort*/);
//        prot.broadcast_USER_ON();
    }


    //todo SOOSSSS!!!TO LOG_ON KAI OLA TA ALLA MESSAGES THA TA STELNW SE ALLA SOCKETS
    //TODO SAYTA POU KATHE PROCESS KANIE LISTEN KAI DEXETAI CONNECTION
    //TODO ARA PREPEI NA KANW ESTABLISH KAINOURGIO CONNECTION OPOU SERVER GINETAI CLIENT
    //TODO KAI CLIENT-SERVER

//    int sock2respondLOG_ON;
//    struct sockaddr_in clientName;
//
//    /* Create socket */
//    if ((sock2respondLOG_ON = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//        perror_exit("socket");
//
//    /* Connect to the server. */
//    init_sockaddr(&clientName, clientIp, clientPort);
//
//
//    if (0 > connect (sock2respondLOG_ON,(struct sockaddr *) &clientName,sizeof (clientName)))
//    {
//        perror ("connect (client)");
//        exit (EXIT_FAILURE);
//    }



    return -1;

}
