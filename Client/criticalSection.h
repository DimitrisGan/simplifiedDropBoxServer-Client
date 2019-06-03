//
// Created by dimitrisgan on 31/5/2019.
//

#ifndef CLIENT_CRITICALSECTION_H
#define CLIENT_CRITICALSECTION_H


#include "mylinkedList.h"
#include "clientTuple.h"
#include "circularBuffer.h"
#include "assistantFunctions.h"


//linkedList<info> test;

static volatile sig_atomic_t  quitThread=0;



struct CS {
    linkedList<clientsTuple> clients_list; //todo tha ginei global gia na einai shared gia ola ta threads!!!
    circularBuffer *circBuffer;

    myString inputDir;

    CS(circularBuffer *circBuffer,  myString inputDir);

    virtual ~CS();

    pthread_mutex_t client_list_mtx;         /* Mutex for synchronization */

//    lock_client_list();
//    unlock_client_list();



};


struct thread_protocol{

    void send_GET_FILE_LIST_and_recv_FILE_LIST(uint32_t ipB, uint16_t portB, linkedList<info> &retNewPaths_list);

    void send_GET_FILE_and_recv(info item,myString inDir);


};

void* worker_function(void* shared);

myString createNewDirName(uint32_t ipB, uint16_t portB);

myString createPathForNewDir (myString inDir , myString nameNewDir);


// This assumes buffer is at least x bytes long,
// and that the socket is blocking.
void ReadXBytes(int socket, unsigned int x, void* buffer ,const char* error_m);


void getAllHigherPaths(myString path2break ,linkedList<myString> & retAllPaths_list);

#endif //CLIENT_CRITICALSECTION_H
