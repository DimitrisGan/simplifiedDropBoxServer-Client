//
// Created by dimitrisgan on 25/5/2019.
//

#include "serverProtocol.h"


void start_session(myString buffer2send){

    cout << "anoigw session-socket gia na milhsw ston client\n";

}




//======================================================

Protocol::Protocol(ArgumentsKeeper args) : args(args) {}


int Protocol::send_header(int sock) {

//    struct sockaddr_in sa;
//
//    myString myIp;myIp = getMyIpInStr();
//
//    cout << "H IP MOU FILARAKI EINAI H : "<<myIp<<endl;
//
//    uint32_t ipInbinary = convertStringIpToBinary(myIp);
//
//
//    if (write(sock, &ipInbinary , sizeof(uint32_t)) < 0)
//        perror_exit("write IP in LOG_ON");
//
//
//    uint16_t portBinary = htons(static_cast<uint16_t>(this->args.portNum.to_int()));
//
//    if (write(sock, &portBinary , sizeof(uint16_t)) < 0)
//        perror_exit("write PORT in LOG_ON");


    return 0;
}

int Protocol::recv_header(int filedes, clientsTuple &tupl) {


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



    return 0;
}



int Protocol::recv_LOG_ON(int filedes, clientsTuple &tupl) {

    cout<< "INFO_SERVER::Receive LOG_ON\n";

    recv_header(filedes, tupl);

    return 0;
}



int Protocol::add_newClient(const clientsTuple & tupl) {

    /*save them to the list of tuples if they dont already exist*/

    if (! this->clients_list.exists(tupl))
        this->clients_list.insert_last(tupl);


    return 0;
}


//////////////////////////////////////////////////






int Protocol::broadcast_USER_ON(const clientsTuple &tupl) {

    cout<< "INFO_SERVER::Broadcast USER_ON\n";

    myString userOn("USER_ON");

    myString ipStr;

    for (auto &clientExist : this->clients_list) {

//        TODO PUT OUT OF COMMENTS!!!
//        if (clientExist == newClientToAnnounce)
//            continue;

        ipStr = convertBinaryIpToString(clientExist.ip);


//        printf("message will be sent to client with ip:: %s \n",ipStr.getMyStr());
//        printf(" and port:: %d \n",clientExist.port);

//        cout << "SEND USER_ON to client--> ip: " << ipStr << " port: " << clientExist.port<<endl;

        int newsock = create_socket_and_connect(ipStr,clientExist.port);

        if (write(newsock , userOn.getMyStr() , userOn.size()) < 0)
            perror_exit("write USER_ON");

        uint32_t ipToSend = htonl(tupl.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP");

        uint16_t portToSend = htons(tupl.port);

        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT");

        close(newsock);
    }

    return 0;
}

int Protocol::recv_GET_CLIENTS(int filedes, clientsTuple &tupl)
{
    cout<< "INFO_SERVER::Receive GET_CLIENTS\n";


    this->recv_header(filedes,tupl);

    close(filedes);
    return 0;
}




int Protocol::send_CLIENTS_LIST(const clientsTuple & tupl) {
    myString ipStr;
    ipStr = convertBinaryIpToString(tupl.ip);


    cout<< "INFO_SERVER::Send CLIENT_LIST to client with ip: "<<ipStr<<" & port: "<< tupl.port<<"\n";

    myString clientList("CLIENT_LIST");

    int newsock = create_socket_and_connect(ipStr,tupl.port);


    if (write(newsock,clientList.getMyStr(), clientList.size()) < 0)
        perror_exit("write  CLIENT_LIST in CLIENT_LIST");

    int N = clients_list.getSize();

    if (write(newsock,&N, sizeof(int)) < 0)
        perror_exit("write N in CLIENT_LIST");

    if (N == 0){
        close(newsock);
        return 0;
    }

    for (auto &clientExist : this->clients_list) {


        ipStr = convertBinaryIpToString(clientExist.ip);

//        printf("message will be sent to client with ip:: %s \n",ipStr.getMyStr());
//        printf(" and port:: %d \n",clientExist.port);



        uint32_t ipToSend = htonl(clientExist.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP in CLIENT_LIST");

//        cout <<"portToSend  ---> "<< clientExist.port <<endl;

        uint16_t portToSend = htons(clientExist.port);

        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT in CLIENT_LIST");

    }

    close(newsock);

    return 0;
}


int Protocol::recv_LOG_OFF(int filedes, clientsTuple &tupl) {

    cout<< "INFO_SERVER::Receive LOG_OFF\n";
//    cout<< "FROM CLIENT:: "<< tupl << "\n";


    recv_header(filedes, tupl);



    return 0;
}









int Protocol::broadcast_USER_OFF(clientsTuple &tupl) {
    //TODO KANEI BROADCAST PANTOU KAI O CLIENT POU VLEPEI OTI AFORA TON IDIO APLA TO AGNOEI
    //TODO OPOTE STELNW KAI TO <IP,PORT>

    cout<< "INFO_SERVER::Broadcast USER_OFF\n";

    myString userOff("USER_OFF");

    myString ipStr;

    for (auto &clientExist : this->clients_list) {

//        if (clientExist == newClientToAnnounce)
//            continue;

        ipStr = convertBinaryIpToString(clientExist.ip);
        printf(" and port:: %d \n",clientExist.ip);


//        printf("message will be sent to client with ip:: %s \n",ipStr.getMyStr());
//        printf(" and port:: %d \n",clientExist.port);

        int newsock = create_socket_and_connect(ipStr,clientExist.port);

        if (write(newsock , userOff.getMyStr() , userOff.size()) < 0)
            perror_exit("write USER_OFF in USER_OFF ");


        uint32_t ipToSend = htonl(tupl.ip);
        if (write(newsock, &ipToSend , sizeof(uint32_t)) < 0)
            perror_exit("write  IP in USER_OFF");


        uint16_t portToSend = htons(tupl.port);

        if (write(newsock, &portToSend , sizeof(uint16_t)) < 0)
            perror_exit("write  PORT in USER_OFF");

        close(newsock);
    }

    return 0;
}

int Protocol::remove_client(clientsTuple &tupl) {

    if (! this->clients_list.exists(tupl) ) //if Client to remove doesn't exist !
        return 1;
    /*Remove the client that sent LOG_OFF*/
    this->clients_list.deleteNodeByItem(tupl);
    return 0;
}






