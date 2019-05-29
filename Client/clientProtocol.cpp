//
// Created by dimitrisgan on 25/5/2019.
//

#include "clientProtocol.h"
#include "socketManipulation.h"

Protocol::Protocol(ArgumentsKeeper args) : args(args) {}


int Protocol::recv_USER_ON(int sock , clientsTuple &newClient){


    uint32_t ipAddr;
    if (read(sock, &ipAddr, sizeof(uint32_t)) < 0) //todo needs while () defensive programming
        perror_exit("read Ip in USER_ON");

    uint16_t  readClientsPort;
    if (read(sock, &readClientsPort, sizeof(uint16_t)) < 0) //todo needs while () defensive programming
        perror_exit("read port in LOG ON");


    uint32_t retIp;
    uint16_t retPort;
    retIp = ntohl(ipAddr);
    retPort = ntohs(readClientsPort);

    newClient.ip     = retIp;
    newClient.port   = retPort;

    cout << "NEW CLIENT :: ...ip = "<<newClient.ip<< " and port = "<<newClient.port<<endl;


    return 0;
}




int Protocol::send_LOG_ON(int sock) {
    cout<< "INFO::Send LOG_ON to server\n";


    myString logOn("LOG_ON");

//    instruction2send+= zip_it(this->args.)


    if (write(sock,logOn.getMyStr(), logOn.size()) < 0)
        perror_exit("write LOG ON");

    send_header(sock);

    close(sock);  /* Close socket and exit */
    return 0;
}


int Protocol::send_header(int sock) {

    struct sockaddr_in sa;

    myString myIp;myIp = getMyIpInStr();

    cout << "H IP MOU FILARAKI EINAI H : "<<myIp<<endl;

    uint32_t ipInbinary = convertStringIpToBinary(myIp);


    if (write(sock, &ipInbinary , sizeof(uint32_t)) < 0)
        perror_exit("write IP in LOG_ON");


    uint16_t portBinary = htons(static_cast<uint16_t>(this->args.portNum.to_int()));

    if (write(sock, &portBinary , sizeof(uint16_t)) < 0)
        perror_exit("write PORT in LOG_ON");


    return 0;
}


int Protocol::send_GET_CLIENTS(int sock) {
    myString getClients("GET_CLIENTS");


    if (write(sock,getClients.getMyStr(), getClients.size()) < 0)
        perror_exit("write GET_CLIENTS");


    this->send_header(sock);


    return 0;
}





int Protocol::send_LOG_OFF(int sock) {

    myString logOff("LOG_OFF");

    if (write(sock,logOff.getMyStr(), logOff.size()) < 0)
        perror_exit("write LOG_OFF");



    return 0;
}

//int Protocol::recv_CLIENTS_LIST() {
//
//    //todo prosoxh! na kanw ntoh() otan lavw ip kai port
//    return 0;
//}





int Protocol::recv_CLIENTS_LIST(int sock, linkedList<clientsTuple> &existingClients_list) {




    int N;
    if (read(sock, &N, sizeof(int)) < 0) //todo needs while () defensive programming
        perror_exit("read Ν in CLIENTS_LIST");


    for (int i = 0; i < N ; ++i) {

        uint32_t  ipRead;
        uint16_t  portRead;


        if (read(sock, &ipRead, sizeof(uint32_t)) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ip in CLIENTS_LIST");

        if (read(sock, &portRead, sizeof(uint16_t)) < 0) //todo needs while () defensive programming
            perror_exit("read i-th port in CLIENTS_LIST");

        uint32_t retIp;
        uint16_t retPort;
        retIp = ntohl(ipRead);
        retPort = ntohs(portRead);

        clientsTuple client(retIp,retPort);

        existingClients_list.insert_last(client);

    }


    return 0;
}




int Protocol::add_newClient(const clientsTuple &tupl) {
    /*save them to the list of tuples if they dont already exist*/

    if (! this->client_list.exists(tupl))
        this->client_list.insert_last(tupl);


    return 0;
}

int Protocol::add_list_of_existing_clients(linkedList<clientsTuple> &existingClients_list) {

    for (auto &tupl : existingClients_list) {
        this->add_newClient(tupl);
    }
    return 0;
}





//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

