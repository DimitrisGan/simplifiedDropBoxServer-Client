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

    void setPathName(myString pathName);

    void setVersion(unsigned int version);

    friend ostream &operator<<(ostream &os, const info &info1);


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


#endif //CLIENT_THREADPOOL_H
