//
// Created by dimitrisgan on 31/5/2019.
//

#ifndef CLIENT_THREADPOOL_H
#define CLIENT_THREADPOOL_H

#include <stdio.h> 		// from www.mario-konrad.ch
#include <pthread.h>
#include <unistd.h>
#include "myString.h"

#define POOL_SIZE 6


struct info{
    myString ip;
    myString port;

    myString pathName;
    unsigned version {0};

    info( myString &ip,  myString &port,  myString &pathName, unsigned int version);
    info();

    bool operator==(const info &rhs) const;

    bool operator!=(const info &rhs) const;

};

struct circularBuffer{
    info* buff;
    int start;
    int end;
    int count;

    explicit circularBuffer(int bufferSize);
    virtual ~circularBuffer();


    //todo needs mutexes,condition variables




};

//void initializeWorkerThreads(workerThrsIds_array);



//void initialize(pool_t * pool);
//
//
//void * producer(void * ptr);
//void place(pool_t * pool, int data);
//
//
//void * consumer(void * ptr);
//int obtain(pool_t * pool);


#endif //CLIENT_THREADPOOL_H
