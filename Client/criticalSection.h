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

    CS(circularBuffer *circBuffer);

    pthread_mutex_t mtx_for_circular_buf;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_nonfull;
};


void* worker_function(void* args);




#endif //CLIENT_CRITICALSECTION_H
