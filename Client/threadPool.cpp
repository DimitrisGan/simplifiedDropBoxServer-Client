//
// Created by dimitrisgan on 31/5/2019.
//

#include "threadPool.h"



circularBuffer::circularBuffer(int bufferSize) {

    this->buff = new info[bufferSize];
    this->start = 0;
    this->end = -1;
    this->count = 0;
}

circularBuffer::~circularBuffer() {

    delete [] this->buff;
    this->buff = nullptr;
}


//
//void initialize(pool_t * pool) {
//    pool->start = 0;
//    pool->end = -1;
//    pool->count = 0;
//}
//
//
//void place(pool_t * pool, int data) {
//    pthread_mutex_lock(&mtx);
//
//    while (pool->count >= POOL_SIZE) {
//        printf(">> Found Buffer Full \n");
//        pthread_cond_wait(&cond_nonfull, &mtx);
//    }
//    pool->end = (pool->end + 1) % POOL_SIZE;
//    pool->data[pool->end] = data;
//    pool->count++;
//
//    pthread_mutex_unlock(&mtx);
//}
//
//
//int obtain(pool_t * pool) {
//    int data = 0;
//    pthread_mutex_lock(&mtx);
//
//    while (pool->count <= 0) {
//        printf(">> Found Buffer Empty \n");
//        pthread_cond_wait(&cond_nonempty, &mtx);
//    }
//    data = pool->data[pool->start];
//    pool->start = (pool->start + 1) % POOL_SIZE;
//    pool->count--;
//
//    pthread_mutex_unlock(&mtx);
//    return data;
//}

//
//void * producer(void * ptr)
//{
//    while (num_of_items > 0) {
//        place(&pool, num_of_items);
//        printf("producer: %d\n", num_of_items);
//        num_of_items--;
//        pthread_cond_signal(&cond_nonempty);
//        usleep(1000);
//    }
//    pthread_exit(0);
//}
//
//
//
//void * consumer(void * ptr)
//{
//    while (num_of_items > 0 || pool.count > 0) {
//        printf("consumer: %d\n", obtain(&pool));
//        pthread_cond_signal(&cond_nonfull);
//        usleep(500000);
//    }
//    pthread_exit(0);
//}
info::info( myString &ip,  myString &port,  myString &pathName, unsigned int version) : ip(ip),
                                                                                                       port(port),
                                                                                                       pathName(
                                                                                                               pathName),
                                                                                                       version(version) {}

info::info() {}

bool info::operator==(const info &rhs) const {
    return ip == rhs.ip &&
           port == rhs.port &&
           pathName == rhs.pathName &&
           version == rhs.version;
}

bool info::operator!=(const info &rhs) const {
    return !(rhs == *this);
}
