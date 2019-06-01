//
// Created by dimitrisgan on 31/5/2019.
//

#include <assert.h>
#include "criticalSection.h"


CS::CS(circularBuffer *circBuffer) : circBuffer(circBuffer) {

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


//    cout << ((CS *)shared)->clients_list<<endl;
//
//    usleep(10);
//    cout << "THREAD PRINT ATTEMPT #2"<<"\t";
//    cout << ((CS *)shared)->clients_list<<endl;
//
//    usleep(10);
//    cout << "THREAD PRINT ATTEMPT #3"<<"\t";
//    cout << ((CS *)shared)->clients_list<<endl;




//    pthread_exit(nullptr);


    pthread_exit(0);

}

void thread_protocol::send_GET_FILE_LIST_and_recv_FILE_LIST(int sock) {
    myString getFileList("1_GET_FILE_LIST");

    if (write(sock, getFileList.getMyStr() ,getFileList.size()) < 0)
        perror_exit("write i-th file_path_name in FILE_LIST");

    myString FILE_LIST("FILE_LIST");
    char ass[9];
    if (read(sock, ass, FILE_LIST.size()) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ip in CLIENTS_LIST");

    assert(FILE_LIST == ass);

    int n;
    if (read(sock, &n, sizeof(n)) < 0) //todo needs while () defensive programming
        perror_exit("read i-th ip in CLIENTS_LIST");


    for (int i = 0; i < n ; ++i) {
        char buf[128];
        if (read(sock, buf, 128) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ip in CLIENTS_LIST");

        myString pathName(buf);
        cout << "PATH = "<<buf<<endl;
        unsigned version;
        if (read(sock, &version, sizeof(unsigned)) < 0) //todo needs while () defensive programming
            perror_exit("read i-th ip in CLIENTS_LIST");

        cout << "VERSION = "<<version<<endl;
        //todo na ftiaksw buffer pou na ta xwnei mesa + na paizw me ta versions =0 an den uparxei hdh

        //TODO
//        info infoTupl();
//        if (test.)
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
