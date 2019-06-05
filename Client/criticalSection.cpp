//
// Created by dimitrisgan on 31/5/2019.
//

#include <assert.h>
#include "criticalSection.h"
#include "socketManipulation.h"
#include "hashFunction.h"


CS::CS(circularBuffer *circBuffer,  myString inputDir) : circBuffer(circBuffer), inputDir(inputDir) {

    this->stop =false;

    pthread_mutex_init(&this->client_list_mtx, NULL);

    pthread_mutex_init(&this->mkdir_mtx, NULL);

    pthread_mutex_init(&this->stop_mtw, NULL);


}


CS::~CS() {
    if (pthread_mutex_destroy(&this->client_list_mtx)) { /* Destroy mutex */
        perror_exit("pthread_mutex_destroy");
    }

    if (pthread_mutex_destroy(&this->mkdir_mtx)) { /* Destroy mutex */
        perror_exit("pthread_mutex_destroy");
    }

    if (pthread_mutex_destroy(&this->stop_mtw)) { /* Destroy mutex */
        perror_exit("pthread_mutex_destroy");
    }

}

bool CS::isStopTime() {
    bool flagStop = false;

    if (pthread_mutex_lock(&this->stop_mtw))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    if (this->stop)
        flagStop = true;

    if (pthread_mutex_unlock(&this->stop_mtw))  /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");


    return flagStop;
}


void* worker_function(void* shared){

    thread_protocol thr;

    while (true) {


        info cbuff_item;


        cbuff_item = ((CS *)shared)->circBuffer->obtain();


        if (cbuff_item.isNewClient()){


            myString newClientsDir; newClientsDir  = newDirName(cbuff_item.ip, cbuff_item.port);
            myString newClientsDirPath("./") ;newClientsDirPath = createPathForNewDir(  ((CS *)shared)->inputDir ,newClientsDir);


            if (! directoryExist(newClientsDirPath.getMyStr()))
                createDirectory(newClientsDirPath.getMyStr());

            linkedList <info> newItems2place_list;


            thr.send_GET_FILE_LIST_and_recv_FILE_LIST(cbuff_item.ip , cbuff_item.port,
                                                      newItems2place_list);

            //add the prefix ip_port/.. in all files
            myString prefix = newClientsDirPath; prefix+="/";

            for (auto &item : newItems2place_list) {

                myString realPath = prefix; realPath+=item.pathName;
                item.setPathName(realPath);


                if (! fileExist(realPath.getMyStr()) && ! directoryExist(realPath.getMyStr())){
                    item.setVersion(0);
                }

                ((CS *)shared)->circBuffer->place(item);

            }

        }

        if (cbuff_item.isFilePath()){

            if (clientStillExist(shared,cbuff_item)){ //if clients still exist in system the proceed
                thr.send_GET_FILE_and_recv(cbuff_item, ((CS *) shared)->inputDir, &shared);
            } //else no need to do anything becauce client is out
            else{
                //do nothing..
            }

        }

    }

    pthread_exit(0);

}


bool clientStillExist(void *shared, info cbuff_item) {
    bool flagExist = false;
    if (pthread_mutex_lock(&((CS *) shared)->client_list_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");


    clientsTuple tupl(cbuff_item.ip, cbuff_item.port);
    if (((CS *) shared)->clients_list.exists(tupl)) { //clients still exists
        flagExist = true;
    } else { //client doesnt exist anymore

    }

    if (pthread_mutex_unlock(&((CS *) shared)->client_list_mtx)) { /* Unlock mutex */
        perror_exit("pthread_mutex_unlock");
    }

    return flagExist;
}



myString newDirName(uint32_t ipB, uint16_t portB) {
    myString ipStr; ipStr = convertBinaryIpToString(ipB);
    myString portStr ; portStr = convertBInaryPortToString(portB);
    myString newClientsDirName = ipStr; newClientsDirName+="_";newClientsDirName+= portStr ;

    return newClientsDirName;
}

myString createPathForNewDir(myString inDir, myString nameNewDir) {
    myString newClientsDirPath;newClientsDirPath=inDir;
    newClientsDirPath+="/";
    newClientsDirPath += nameNewDir;


    return newClientsDirPath;
}


// This assumes buffer is at least x bytes long,
// and that the socket is blocking.
void ReadXBytes(int socket, unsigned int x, void* buffer ,const char* error_m)
{
    int bytesRead = 0;
    int result;
    while (bytesRead < x)
    {
        result = static_cast<int>(read(socket, buffer + bytesRead , x - bytesRead));

        if (result < 0 )
        {
            // Throw your error.
            perror_exit(error_m);

        }

        bytesRead += result;
    }
}

void getAllHigherPaths(myString path2break, linkedList<myString> &retAllPaths_list) {

    myString subPath("");/*subPath = path2break.getMyStr()[0];*/
    for (int i = 0; i < path2break.size(); ++i) {

        if (path2break.getMyStr()[i] != '/' ){
            myString ch(path2break.getMyStr()[i]);
            subPath+= ch;
        }
        if (path2break.getMyStr()[i] == '/' ){
            myString ch(path2break.getMyStr()[i]);

            retAllPaths_list.insert_last(subPath);
            subPath+=ch;
        }
    }

}




void
thread_protocol::send_GET_FILE_LIST_and_recv_FILE_LIST(uint32_t ipB, uint16_t portB, linkedList<info> &retNewPaths_list) {
    myString getFileList("1_GET_FILE_LIST");

    myString ipStr; ipStr = convertBinaryIpToString(ipB);

    int sock = create_socket_and_connect(ipStr, portB);


    //cout<< "INFO_CLIENT-WORKER!::Send GET_FILE_LIST from send_GET_FILE_LIST_and_recv_FILE_LIST  to other client\n";


    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write i-th file_path_name in FILE_LIST");

    myString FILE_LIST("FILE_LIST");
    char ass[FILE_LIST.size()+1];   memset(ass, 0, sizeof(ass));
    ReadXBytes(sock,FILE_LIST.size(),ass,"read FILE_LIST in CLIENTS_LIST");


    assert(FILE_LIST == ass);

    int n;
    if (read(sock, &n, sizeof(n)) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ipB in CLIENTS_LIST");


    for (int i = 0; i < n ; ++i) {
        char buf[128]; memset(buf, 0, sizeof(buf));


        ReadXBytes(sock, sizeof(buf),buf,"read i-th ipB in CLIENTS_LIST");
        myString pathName(buf);

        unsigned version;
        if (read(sock, &version, sizeof(unsigned)) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ipB in CLIENTS_LIST");


        info info2add(ipB,portB,pathName,version);

        retNewPaths_list.insert_last(info2add);


    }

    close(sock);
}

void thread_protocol::send_GET_FILE_and_recv(info item, myString inDir, void *shared) {

    myString getFileList("GET_FILE");

    myString prefix;prefix = newDirName(item.ip, item.port);

    myString path2send  = item.pathName;
    path2send.removeSubstr(inDir);path2send.removeSubstr("/");path2send.removeSubstr(prefix);
    myString ipStr; ipStr = convertBinaryIpToString(item.ip);


    int sock = create_socket_and_connect(ipStr, item.port);


    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write GET_FILE in GET_FILE");


    if (write(sock, path2send.getMyStr() ,128) < 0)
        perror_exit("write file_path_name in GET_FILE");


    if (! fileExist(item.pathName.getMyStr()) && ! directoryExist(item.pathName.getMyStr()) ) { //if the file doesn't exist send version = 0 to obtain the file


        unsigned zero =0;
        if (write(sock, &zero, sizeof(item.version)) < 0)
            perror_exit("write version in GET_FILE");
    }
    else{ //means that the file/dir exists

        if (is_dir(item.pathName.getMyStr())){
            unsigned one = 1;
            if (write(sock, &one, sizeof(item.version)) < 0)
                perror_exit("write version in GET_FILE");
        }
        else{ //is_regular_file
            myString fileContent;
            loadContextOfFile(item.pathName,fileContent); //load the content

            int version = myHash(fileContent); //and hash it to take the version

            if (write(sock, &version, sizeof(item.version)) < 0)
                perror_exit("write version in GET_FILE");
        }

    }

    linkedList <myString> allPaths;
    getAllHigherPaths( item.pathName,allPaths);
    for (auto &subPath : allPaths) {
        /*for every subdir from upper layer that is not created wait until it will*/
        while (! directoryExist(subPath.getMyStr()))
            usleep(1);
    }

    //-------------------- RECEIVE --------------------

    char buf[1];
    myString instruction("");
    bool flagFILE_UP_TO_DATE    = false;
    bool flagFILE_SIZE          = false;
    bool flagFILE_NOT_FOUND     = false;

    while(read(sock, buf, 1) > 0) {  /* Receive 1 char */

        instruction += buf;

        if (instruction == "FILE_UP_TO_DATE") {
            flagFILE_UP_TO_DATE = true;
            break;
        }

        if (instruction == "FILE_SIZE") {
            flagFILE_SIZE = true;
            break;
        }

        if (instruction == "FILE_NOT_FOUND") {
            flagFILE_NOT_FOUND = true;
            break;
        }

    }

    if (flagFILE_UP_TO_DATE){
        cout<< "INFO_CLIENT-WORKER::Received FILE_UP_TO_DATE for file: "<<item.pathName<<endl;
    }

    if (flagFILE_NOT_FOUND){
        cout<< "INFO_CLIENT-WORKER::Received FILE_NOT_FOUND for file: "<<item.pathName<<endl;
    }

    if (flagFILE_SIZE){

        unsigned versionGiven;
        if (read(sock, &versionGiven, sizeof(versionGiven)) < 0) //todo needs while () defensive programming
            perror_exit("read version in GET_FILE");

        unsigned size;
        if (read(sock, &size, sizeof(unsigned)) < 0) //todo needs while () defensive programming
            perror_exit("read size in GET_FILE");


        if (versionGiven == 1 && size == 0){ //is a dir


            if (! directoryExist(item.pathName.getMyStr())){ //if the dir doesnt exist create it
                createDirectory(item.pathName.getMyStr());
            }

            usleep(1);

        }

        else{ //is a file

            char contentBuf[size+1];
            memset(contentBuf, 0, sizeof(contentBuf));

            ReadXBytes(sock, size, contentBuf ,"read content in GET_FILE");

            //create the file
            FILE *fp;
            /*writes in received file*/
            fp = fopen(item.pathName.getMyStr(), "a"); //creates OR appends the file in mirror dir
            fprintf(fp, "%s", contentBuf); //writes the content of the file
            fclose(fp);

        }

    }

    close(sock);

}
