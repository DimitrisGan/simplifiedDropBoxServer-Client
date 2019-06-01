//
// Created by dimitrisgan on 31/5/2019.
//

#include "circularBuffer.h"
#include "assistantFunctions.h"


circularBuffer::circularBuffer(int bufferSize) {

    this->data = new info[bufferSize];

    this->start = 0;
    this->end = -1;
    this->count = 0;
    this->buffSize = bufferSize;

    pthread_mutex_init(&this->circular_buf_mtx, NULL);
    pthread_cond_init(&cond_nonempty, 0);
    pthread_cond_init(&cond_nonfull, 0);

}

circularBuffer::~circularBuffer() {

    delete [] this->data;
    this->data = nullptr;

    if (pthread_mutex_destroy(&this->circular_buf_mtx)) { /* Destroy mutex */
        perror_exit("pthread_mutex_destroy");
    }

    pthread_cond_destroy(&cond_nonempty);
    pthread_cond_destroy(&cond_nonfull);
}


//
//void initialize(pool_t * pool) {
//    pool->start = 0;
//    pool->end = -1;
//    pool->count = 0;
//}


void circularBuffer::place(info data) {

    if (pthread_mutex_lock(&this->circular_buf_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    while (this->count >= this->buffSize) {
        printf(">> Found circBuffer Full \n");
        pthread_cond_wait(&this->cond_nonfull, &this->circular_buf_mtx);
    }
    this->end = (this->end + 1) % buffSize;
    this->data[this->end] = data;
    this->count++;

    if (pthread_mutex_unlock(&this->circular_buf_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");
}



info circularBuffer::obtain() {
    info data ;
    pthread_mutex_lock(&this->circular_buf_mtx);

    while (this->count <= 0) {
        printf(">> Found Buffer Empty \n");
        pthread_cond_wait(&cond_nonempty, &this->circular_buf_mtx);
    }
    data = this->data[this->start];
    this->start = (this->start + 1) % POOL_SIZE;
    this->count--;

    pthread_mutex_unlock(&this->circular_buf_mtx);
    return data;
}


void * producer(/*void * ptr*/)
{

    //TODO LOGIKA OLO AUTO DE XREIAZETAI GT THA EXW ASUGXRONA AITHMATA POU THA THELOUN NA XWSOUN STO BUFFER
    //TODO ISWS EDW NA TO XWSW APO TO client_list GENIKA SKEPSOU TO OTAN GURISEIS
    while (num_of_items > 0) {
        place(&pool, num_of_items);
        printf("producer: %d\n", num_of_items);
        num_of_items--;
        pthread_cond_signal(&cond_nonempty);
//        usleep(1000);
    }
    pthread_exit(0);
}

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
