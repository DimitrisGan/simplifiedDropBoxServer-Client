//
// Created by dimitrisgan on 31/5/2019.
//

#ifndef CLIENT_THREADPOOL_H
#define CLIENT_THREADPOOL_H

#include <stdio.h> 		// from www.mario-konrad.ch
#include <pthread.h>
#include <unistd.h>
#include <ostream>
#include "myString.h"
#include "clientTuple.h"

#define POOL_SIZE 6


struct info{
    uint32_t ip;
    uint16_t port;

    myString pathName;
    unsigned version {0};

    info(uint32_t ip, uint16_t port, myString &pathName, unsigned int version);

    info();

    bool operator==(const info &rhs) const;

    bool operator!=(const info &rhs) const;

    void prepareNewClient(clientsTuple tupl);

    bool isNewClient();
    bool isFilePath();
    bool isFile();

    bool isDir();

    friend ostream &operator<<(ostream &os, const info &info1);

//    info operator=( info &right) ;   // for assign with myString


};

struct circularBuffer{
    info* data;
    int start;
    int end;
    int count;

    int buffSize;

    pthread_mutex_t circular_buf_mtx;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_nonfull;


    explicit circularBuffer(int bufferSize);
    virtual ~circularBuffer();

    bool isFull();
    bool isEmpty();

    void place(info data);
    info obtain();

    int size();


};

//void initializeWorkerThreads(workerThrsIds_array);



//
//
//void * producer(void * ptr);
//void place(pool_t * pool, int data);
//
//
//void * consumer(void * ptr);
//int obtain(pool_t * pool);


#endif //CLIENT_THREADPOOL_H
