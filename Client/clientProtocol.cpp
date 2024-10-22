//
// Created by dimitrisgan on 25/5/2019.
//

#include "clientProtocol.h"

Protocol::Protocol(ArgumentsKeeper args) : args(args) {

    list_all_in_dir(args.inDir, this->initFilesInDir_list);

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


    if (write(sock,logOn.getMyStr(), logOn.size()) < 0)
        perror_exit("write LOG ON");

    send_header(sock);

    close(sock);  /* Close socket and exit */
    return 0;
}


int Protocol::send_header(int sock) {

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


    if (isMe(tupl)) //if its me dont add it
        return 0;

    if (pthread_mutex_lock(&shared.client_list_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");


    if (! shared.clients_list.exists(tupl))
        shared.clients_list.insert_last(tupl);

    if (pthread_mutex_unlock(&shared.client_list_mtx)) { /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");
    }

    //==================================

    info tuplInCircBuffer;
    tuplInCircBuffer.prepareNewClient(tupl);

    shared.circBuffer->place(tuplInCircBuffer); /*place info tuple to circular buffer*/

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

    if (! shared.clients_list.exists(tupl) )
        perror_exit("Client doesn't exist to remove!");

    shared.clients_list.deleteNodeByItem(tupl); /*Remove the client that sent LOG_OFF*/

    if (pthread_mutex_unlock(&shared.client_list_mtx)) { /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");
    }

    return 0;
}


int Protocol::recv_GET_FILE_LIST(int sock) {
    cout<< "INFO_CLIENT::Receive GET_FILE_LIST from another client's worker_thread\n";
    return 0;
}



int Protocol::respond_with_FILE_LIST(int sock) {
    cout<< "INFO_CLIENT::Send/respond FILE_LIST to another client's worker_thread\n";

    myString fileList("FILE_LIST");

    /*write FILE_LIST*/
    if (write(sock,fileList.getMyStr(), fileList.size()) < 0)
        perror_exit("write  FILE_LIST in FILE_LIST");

    int n = this->initFilesInDir_list.getSize();
    /*write n = number of file names*/
    if (write(sock, &n ,sizeof(int)) < 0)
        perror_exit("write n in FILE_LIST");

    myString deletePrefix = this->args.inDir; deletePrefix+="/";

    for (auto &filePath : this->initFilesInDir_list) {
        myString filePath2send = filePath;
        filePath2send.removeSubstr(deletePrefix.getMyStr());

        if (write(sock, filePath2send.getMyStr() ,128) < 0)
            perror_exit("write i-th file_path_name in FILE_LIST");

        unsigned version ;

        if (is_regular_file(filePath.getMyStr())) { //if it's regular file

            myString fileContent;
            loadContextOfFile(filePath,fileContent); //load the content

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
//    cout<< "INFO_CLIENT::Receive GET_FILE #1 from another client - worker_thread\n";

    return 0;
}

int Protocol::respond_to_GET_FILE(int sock) {
//    cout<< "INFO_CLIENT::Receive GET_FILE #2 from another client - worker_thread\n";
//    cout<< "INFO_CLIENT::Respond to GET_FILE to another client's worker_thread\n";


    char buf[128]; memset(buf, 0, sizeof(buf));

//    ReadXBytes(sock, sizeof(buf),buf,"read pathname in GET_FILE");

    if (read(sock, buf, 128) < 0) //todo needs while () defensive programming
        perror_exit("read pathname in GET_FILE");

//    ReadXBytes(sock,128,buf,"read pathname in GET_FILE");

    myString pathNameAsked(buf);
    unsigned version_other;

    if (read(sock, &version_other, sizeof(unsigned)) < 0) //todo needs while () defensive programming
        perror_exit("read version_other in GET_FILE");


    myString realPath = this->args.inDir;   realPath+=pathNameAsked;

    if (fileExist(realPath.getMyStr()) || directoryExist(realPath.getMyStr())){
        if (is_dir(realPath.getMyStr())){ //if its dir
            // send  FILE filesize byte0byte1..byten
            //send version_other=1
            //send bytes = 0
            myString fileHeader("FILE_SIZE");
            if (write(sock, fileHeader.getMyStr() , fileHeader.size()) < 0)
                perror_exit("write FILE_SIZE in GET_FILE");

            unsigned version1=1;
            if (write(sock, &version1 , sizeof(unsigned)) < 0)
                perror_exit("write version1 in GET_FILE");


            unsigned filesize=0;
            if (write(sock, &filesize , sizeof(unsigned)) < 0)
                perror_exit("write fileSize in GET_FILE");




        }
        else{ //is a file
            //chech version_other with hash;
            myString fileContent;
            loadContextOfFile(realPath,fileContent); //load the content
            unsigned  version_mine = myHash(fileContent); //and hash it to take the version_other

            if (version_mine != version_other){ //if has a previous version then send  FILE filesize byte0byte1..byten


                myString fileHeader("FILE_SIZE");
                if (write(sock, fileHeader.getMyStr() , fileHeader.size()) < 0)
                    perror_exit("write FILE_SIZE in GET_FILE");

                if (write(sock, &version_mine , sizeof(unsigned)) < 0)
                    perror_exit("write version in GET_FILE");

                unsigned fileSize = fileContent.size();
                if (write(sock, &fileSize , sizeof(unsigned)) < 0)
                    perror_exit("write version1 in GET_FILE");


                if (write(sock, fileContent.getMyStr() , fileContent.size()) < 0)
                    perror_exit("write byte in GET_FILE");


            }


            else{ //are equal so the file asked is up to date

                myString fileUpToDate("FILE_UP_TO_DATE");
                if (write(sock, fileUpToDate.getMyStr() ,fileUpToDate.size()) < 0)
                    perror_exit("write FILE_UP_TO_DATE in GET_FILE");

            }

        }


    }
    else{ //means that doesn't exist the file so send FILE_NOT_FOUND
        myString fileNotFound("FILE_NOT_FOUND");
        if (write(sock, fileNotFound.getMyStr() ,fileNotFound.size()) < 0)
            perror_exit("write FILE_NOT_FOUND in GET_FILE");

    }


    close(sock);
    return 0;
}

bool Protocol::isMe(const clientsTuple &tupl) {

    uint16_t  myport = static_cast<uint16_t>(this->args.portNum.to_int());
    myString myIpStr;myIpStr = getMyIpInStr();
    uint32_t  myIpB = convertStringIpToBinary(myIpStr);

    return myport == tupl.port && myIpB == tupl.ip; //if its me return true

}


