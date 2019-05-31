//
// Created by dimitrisgan on 31/5/2019.
//

#ifndef CLIENT_CRITICALSECTION_H
#define CLIENT_CRITICALSECTION_H


#include "mylinkedList.h"
#include "clientTuple.h"
#include "threadPool.h"

struct CriticalSection {
    linkedList<clientsTuple> clients_list; //todo tha ginei global gia na einai shared gia ola ta threads!!!
    circularBuffer circBuffer;

    explicit CriticalSection(const circularBuffer &circBuffer);

    pthread_mutex_t mtx_for_circular_buf;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_nonfull;
};







#endif //CLIENT_CRITICALSECTION_H
