//
// Created by dimitrisgan on 31/5/2019.
//

#ifndef CLIENT_THREADPOOL_H
#define CLIENT_THREADPOOL_H

#include <stdio.h> 		// from www.mario-konrad.ch
#include <pthread.h>
#include <unistd.h>

#define POOL_SIZE 6

typedef struct {
    int data[POOL_SIZE];
    int start;
    int end;
    int count;
} pool_t;


pthread_mutex_t mtx;
pthread_cond_t cond_nonempty;
pthread_cond_t cond_nonfull;
pool_t pool;

void initialize(pool_t * pool);


void * producer(void * ptr);
void place(pool_t * pool, int data);


void * consumer(void * ptr);
int obtain(pool_t * pool);


#endif //CLIENT_THREADPOOL_H
