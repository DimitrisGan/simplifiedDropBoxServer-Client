//
// Created by dimitrisgan on 31/5/2019.
//

#ifndef CLIENT_CRITICALSECTION_H
#define CLIENT_CRITICALSECTION_H


#include "mylinkedList.h"
#include "clientTuple.h"
#include "threadPool.h"

struct CS {
    linkedList<clientsTuple> clients_list; //todo tha ginei global gia na einai shared gia ola ta threads!!!
    circularBuffer *circBuffer;

    explicit CS(circularBuffer *circBuffer);

    pthread_mutex_t mtx_for_circular_buf;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_nonfull;
};


struct thread_protocol{

  void send_GET_FILE_LIST_and_recv_FILE_LIST(int sock/*,tuple gia th na diavasei*/);

    void send_GET_FILE_and_recv(int sock, myString filePath, unsigned version);
};

void* worker_function(void* args);




#endif //CLIENT_CRITICALSECTION_H
