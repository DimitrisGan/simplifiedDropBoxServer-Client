//
// Created by dimitrisgan on 25/5/2019.
//

#include "clientProtocol.h"

Protocol::Protocol(ArgumentsKeeper args) : args(args) {

    list_all_in_dir(args.inDir, this->initFilesInDir_list);

    cout << initFilesInDir_list;
}


int Protocol::recv_USER_ON(int sock , clientsTuple &tupl){
    cout<< "INFO_CLIENT::Receive USER_ON from server\n";


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

    tupl.ip     = retIp;
    tupl.port   = retPort;



    return 0;
}




int Protocol::send_LOG_ON(int sock) {
    cout<< "INFO_CLIENT::Send LOG_ON to server\n";


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

    uint32_t ipInbinary = convertStringIpToBinary(myIp);


    if (write(sock, &ipInbinary , sizeof(uint32_t)) < 0)
        perror_exit("write IP in LOG_ON");


    uint16_t portBinary = htons(static_cast<uint16_t>(this->args.portNum.to_int()));

    if (write(sock, &portBinary , sizeof(uint16_t)) < 0)
        perror_exit("write PORT in LOG_ON");


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


int Protocol::send_GET_CLIENTS(int sock) {
    cout<< "INFO_CLIENT::Send GET_CLIENTS to server\n";


    myString getClients("GET_CLIENTS");


    if (write(sock,getClients.getMyStr(), getClients.size()) < 0)
        perror_exit("write GET_CLIENTS");


    this->send_header(sock);

    close(sock);  /* Close socket and exit */

    return 0;
}





int Protocol::send_LOG_OFF(int sock) {

    myString logOff("LOG_OFF");

    if (write(sock,logOff.getMyStr(), logOff.size()) < 0)
        perror_exit("write LOG_OFF");


    this->send_header(sock);

    close(sock);  /* Close socket and exit */

    return 0;
}





int Protocol::recv_CLIENTS_LIST(int sock, linkedList<clientsTuple> &existingClients_list) {

    cout<< "INFO_CLIENT::Receive CLIENTS_LIST from server\n";


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




int Protocol::add_client(const clientsTuple &tupl , CS &shared ) {
    /*save them to the list of tuples if they dont already exist*/


    if (pthread_mutex_lock(&shared.client_list_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    printf("Thread %ld: Locked the mutex\n", pthread_self());

    if (! shared.clients_list.exists(tupl))
        shared.clients_list.insert_last(tupl);

    if (pthread_mutex_unlock(&shared.client_list_mtx)) { /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");
    }

    printf("Thread %ld: Unlocked the mutex\n", pthread_self());

    return 0;
}

int Protocol::add_list_of_existing_clients(linkedList<clientsTuple> &existingClients_list , CS &shared) {

    for (auto &tupl : existingClients_list) {
        this->add_client(tupl,shared);
    }
    return 0;
}

int Protocol::recv_USER_OFF(int sock, clientsTuple &tupl) {


    cout<< "INFO_CLIENT::Receive USER_OFF from server\n";


    recv_header(sock, tupl);


    return 0;
}

int Protocol::remove_client(const clientsTuple &tupl , CS &shared) {

    if (pthread_mutex_lock(&shared.client_list_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    printf("Thread %ld: Locked the mutex\n", pthread_self());

    if (! shared.clients_list.exists(tupl) )
        perror_exit("Client doesn't exist to remove!");

    /*Remove the client that sent LOG_OFF*/
    shared.clients_list.deleteNodeByItem(tupl);


    if (pthread_mutex_unlock(&shared.client_list_mtx)) { /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");
    }

    printf("Thread %ld: Unlocked the mutex\n", pthread_self());


    return 0;
}

int Protocol::recv_GET_FILE_LIST(int sock) {
    cout<< "INFO_CLIENT::Receive GET_FILE_LIST from another client - worker_thread\n";

    //TODO pithanon na th svhsw gt einai axreiasth
//    this->respond_with_FILE_LIST(sock);
    return 0;
}




int Protocol::respond_with_FILE_LIST(int sock) {
    cout<< "INFO_CLIENT::Send/respond FILE_LIST from another client - worker_thread\n";

    myString fileList("FILE_LIST");


    /*write FILE_LIST*/
    if (write(sock,fileList.getMyStr(), fileList.size()) < 0)
        perror_exit("write  FILE_LIST in FILE_LIST");

    int n = this->initFilesInDir_list.getSize();
    /*write n = number of file names*/
    if (write(sock, &n ,sizeof(int)) < 0)
        perror_exit("write n in FILE_LIST");


    //todo stelnw FILE_LIST 128 bytes gia pathname ,

    for (auto &filePath2send : this->initFilesInDir_list) {

//        char buf[128];
//        buf = filePath2send.getMyStr();


        if (write(sock, filePath2send.getMyStr() ,128) < 0)
            perror_exit("write i-th file_path_name in FILE_LIST");


        unsigned version ;

        if (is_regular_file(filePath2send.getMyStr())) { //if it's regular file

            myString fileContent;
            loadContextOfFile(filePath2send,fileContent); //load the content

            version = myHash(fileContent); //and hash it to take the version
        }
        else{  //if it's dir then version = 1
            version = 1;
        }

        if (write(sock, &version ,sizeof(version)) < 0)
            perror_exit("write i-th version in FILE_LIST");


    }



    return 0;
}

int Protocol::recv_GET_FILE(int sock) {
    cout<< "INFO_CLIENT::Receive GET_FILE from another client - worker_thread\n";

    return 0;
}

int Protocol::respond_to_GET_FILE(int sock) {

    char buf[128];
    if (read(sock, buf, 128) < 0) //todo needs while () defensive programming
        perror_exit("read pathname in GET_FILE");

    myString pathNameAsked(buf);
    unsigned version;

    if (read(sock, &version, sizeof(unsigned)) < 0) //todo needs while () defensive programming
        perror_exit("read version in GET_FILE");

    cout << "path asked to give:\t"<< pathNameAsked<<endl;
    cout << "version ?:\t"<<version<<endl;

    return 0;
}



//
//int Protocol::respond_with_FILE_LIST(int sock) {
//    cout<< "INFO_CLIENT::Send/respond FILE_LIST from another client - worker_thread\n";
//
//    myString fileList("FILE_LIST");
//
//
//    /*write FILE_LIST*/
//    if (write(sock,fileList.getMyStr(), fileList.size()) < 0)
//        perror_exit("write  FILE_LIST in FILE_LIST");
//
//    int n = this->initFilesInDir_list.getSize();
//    /*write n = number of file names*/
//    if (write(sock, &n ,sizeof(int)) < 0)
//        perror_exit("write n in FILE_LIST");
//
//
//    //todo stelnw FILE_LIST 128 bytes gia pathname ,
//
//    for (auto &filePath2send : this->initFilesInDir_list) {
//
////        char buf[128];
////        buf = filePath2send.getMyStr();
//
//
//        if (write(sock, filePath2send.getMyStr() ,128) < 0)
//            perror_exit("write i-th file_path_name in FILE_LIST");
//
//
//        unsigned version ;
//
//        if (is_dir(filePath2send.getMyStr())) { //if it's dir then version = 1
//
//            version = 1;
//        }
//        else{   //if it's regular file
//            myString fileContent;
//            loadContextOfFile(filePath2send,fileContent); //load the content
//
//            version = myHash(fileContent); //and hash it to take the version
//            cout << "version after hash = "<<version <<endl;
//        }
//
//        if (write(sock, &version ,sizeof(version)) < 0)
//            perror_exit("write i-th version in FILE_LIST");
//
//
//    }
//
//
//    return 0;
//}



