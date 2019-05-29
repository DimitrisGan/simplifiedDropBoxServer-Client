//
// Created by dimitrisgan on 25/5/2019.
//

#include "serverProtocol.h"


void start_session(myString buffer2send){

    cout << "anoigw session-socket gia na milhsw ston client\n";

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


    cout << "...ip = "<<tupl.ip<< " and port = "<<tupl.port<<endl;

    return 0;
}



int Protocol::add_newClient(const clientsTuple & tupl) {

    /*save them to the list of tuples if they dont already exist*/

    if (! this->connectedClients_list.exists(tupl))
        this->connectedClients_list.insert_last(tupl);


    return 0;
}


//////////////////////////////////////////////////






int Protocol::broadcast_USER_ON(const clientsTuple &newClientToAnnounce) {

    cout<< "INFO::Broadcast USER_ON\n";

    myString userOn("USER_ON");

    myString ipStr;

    for (auto &clientExist : this->connectedClients_list) {

//        TODO PUT OUT OF COMMENTS!!!
//        if (clientExist == newClientToAnnounce)
//            continue;

        ipStr = convertBinaryIpToString(clientExist.ip);
        printf(" and port:: %d \n",clientExist.ip);


        printf("message will be sent to client with ip:: %s \n",ipStr.getMyStr());
        printf(" and port:: %d \n",clientExist.port);

        int newsock = create_socket_and_connect(ipStr,clientExist.port);

        if (write(newsock , userOn.getMyStr() , userOn.size()) < 0)
            perror_exit("write USER_ON");

        uint32_t ipToSend = htonl(newClientToAnnounce.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP");

        cout <<"portToSend prin ---> "<< newClientToAnnounce.port <<endl;

        uint16_t portToSend = htons(newClientToAnnounce.port);
        cout <<"portToSend meta ---> "<< portToSend <<endl;

        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT");

        close(newsock);
    }

    return 0;
}

int Protocol::recv_GET_CLIENTS(const clientsTuple & tupl)
{

    send_CLIENTS_LIST(tupl);

    return 0;
}

/////////////////////===================================
/////////////////////===================================
/////////////////////===================================
/////////////////////===================================

int Protocol::send_CLIENTS_LIST(const clientsTuple & tupl) {

    cout<< "INFO::Send CLIENT_LIST\n";

    myString clientList("CLIENT_LIST");


    myString ipStr;
    ipStr = convertBinaryIpToString(tupl.ip);
    int newsock = create_socket_and_connect(ipStr,tupl.port);


    if (write(newsock,clientList.getMyStr(), clientList.size()) < 0)
        perror_exit("write  CLIENT_LIST in CLIENT_LIST");

    int N = connectedClients_list.getSize();

    if (write(newsock,&N, sizeof(int)) < 0)
        perror_exit("write N in CLIENT_LIST");



    for (auto &clientExist : this->connectedClients_list) {

//        TODO PUT OUT OF COMMENTS!!!
//        if (clientExist == newClientToAnnounce)
//            continue;

        ipStr = convertBinaryIpToString(clientExist.ip);
        printf(" and port:: %d \n",clientExist.ip);


        printf("message will be sent to client with ip:: %s \n",ipStr.getMyStr());
        printf(" and port:: %d \n",clientExist.port);



        uint32_t ipToSend = htonl(clientExist.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP in CLIENT_LIST");

        cout <<"portToSend prin ---> "<< clientExist.port <<endl;

        uint16_t portToSend = htons(clientExist.port);
        cout <<"portToSend meta ---> "<< portToSend <<endl;

        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT in CLIENT_LIST");

    }

    close(newsock);

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



