//
// Created by dimitrisgan on 31/5/2019.
//

#include <assert.h>
#include "criticalSection.h"
#include "socketManipulation.h"



CS::CS(circularBuffer *circBuffer,  myString inputDir) : circBuffer(circBuffer), inputDir(inputDir) {

    pthread_mutex_init(&this->client_list_mtx, NULL);

}


CS::~CS() {
    if (pthread_mutex_destroy(&this->client_list_mtx)) { /* Destroy mutex */
        perror_exit("pthread_mutex_destroy");
    }
}



void* worker_function(void* shared){

    cout << "GEIA SOU APO THREAD!!\n";
    cout << "THREAD PRINT ATTEMPT #1"<<"\t";

    thread_protocol thr;
//    int test= ((CS *)shared))

    while (true/*quitThread == 0*/) {
        cout << "edw trww seg? 3\n";

        sleep(3);



//    cout << ((CS *)shared)->clients_list<<endl;
//
//    usleep(10);
//    cout << "THREAD PRINT ATTEMPT #2"<<"\t";
//    cout << ((CS *)shared)->clients_list<<endl;
//
//    usleep(10);
//    cout << "THREAD PRINT ATTEMPT #3"<<"\t";
//    cout << ((CS *)shared)->clients_list<<endl;


        info cbuff_item;


        cout << "CIRCLED BUFFER SIZE IS : "<< ((CS *)shared)->circBuffer->size()<<endl;

        cout << "edw trww seg? 4\n";

        cbuff_item = ((CS *)shared)->circBuffer->obtain();


        cout <<"to cbuff_item exei times "<<cbuff_item<<endl;
        cout << "edw trww seg? 5\n";



        cout << "edw trww seg? 6\n";


        if (cbuff_item.isNewClient()){
            cout << "edw trww seg? 7\n";


            myString newClientsDir; newClientsDir  = createNewDirName(cbuff_item.ip ,cbuff_item.port);
            myString newClientsDirPath;newClientsDirPath = createPathForNewDir(  ((CS *)shared)->inputDir ,newClientsDir);

            cout << "edw trww seg? 8\n";

            createDirectory(newClientsDirPath.getMyStr());

            cout << "edw trww seg? 9\n";


            cout <<"WORKER::new Dir for new Client created in Path = "<<newClientsDirPath<<endl;


            linkedList <info> newItems2place_list;


            thr.send_GET_FILE_LIST_and_recv_FILE_LIST(cbuff_item.ip , cbuff_item.port,
                                                      newItems2place_list);

            //add the prefix ip_port/.. in all files
            myString prefix = newClientsDirPath; prefix+="/";

            for (auto &item : newItems2place_list) {

//                addPrefix2list_of_file(item.pathName);
                myString realPath = prefix; realPath+=item.pathName;
                item.setPathName(realPath);


//                if (item.exists()){}

                cout <<"New paths are:\t";
                cout<<item<<endl;
            }

        }

        if (cbuff_item.isFilePath()){

        }

        if (cbuff_item.isFile()){

        }





//    pthread_exit(nullptr);
    }
    cout << "thread exits!!!!!!!!\n";

    pthread_exit(0);

}

myString createNewDirName(uint32_t ipB, uint16_t portB) {
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


void
thread_protocol::send_GET_FILE_LIST_and_recv_FILE_LIST(uint32_t ipB, uint16_t portB, linkedList<info> &retNewPaths_list) {
    myString getFileList("1_GET_FILE_LIST");

    myString ipStr; ipStr = convertBinaryIpToString(ipB);

    int sock = create_socket_and_connect(ipStr, portB);


    cout<< "INFO_CLIENT-WORKER!::Send GET_FILE_LIST from send_GET_FILE_LIST_and_recv_FILE_LIST  to other client\n";


    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write i-th file_path_name in FILE_LIST");

    myString FILE_LIST("FILE_LIST");
    char ass[9];
    if (read(sock, ass, FILE_LIST.size()) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ipB in CLIENTS_LIST");

    assert(FILE_LIST == ass);

    int n;
    if (read(sock, &n, sizeof(n)) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ipB in CLIENTS_LIST");


    for (int i = 0; i < n ; ++i) {
        char buf[128];
        if (read(sock, buf, 128) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ipB in CLIENTS_LIST");

        myString pathName(buf);
        cout << "PATH = "<<buf<<endl;
        unsigned version;
        if (read(sock, &version, sizeof(unsigned)) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ipB in CLIENTS_LIST");

        cout << "VERSION = "<<version<<endl;



        info info2add(ipB,portB,pathName,version);

        retNewPaths_list.insert_last(info2add);
        //todo na ftiaksw buffer pou na ta xwnei mesa + na paizw me ta versions =0 an den uparxei hdh

        //TODO
//        if (fileExist(this.)) //todo tha to valw sth worker synarthsh kai apo kei tha tsekarw an uparxei hdh gia na allaksw to version
    }


    close(sock);
}

void thread_protocol::send_GET_FILE_and_recv(int sock ,myString filePath, unsigned version) {

    myString getFileList("GET_FILE");

    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write GET_FILE in GET_FILE");

    if (write(sock, filePath.getMyStr() ,128) < 0)
        perror_exit("write file_path_name in GET_FILE");

    //todo FILE_NOT_FOUND
    //todo FILE_UP_TO_DATE



//    close(sock);

}
