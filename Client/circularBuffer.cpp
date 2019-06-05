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



void circularBuffer::place(info data) {

    if (pthread_mutex_lock(&this->circular_buf_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    while (this->isFull()) {
        printf(">> Found circBuffer Full \n");
        pthread_cond_wait(&this->cond_nonfull, &this->circular_buf_mtx);
    }
    this->end = (this->end + 1) % buffSize;
    this->data[this->end] = data;
    this->count++;

    if (pthread_mutex_unlock(&this->circular_buf_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");


    pthread_cond_signal(&this->cond_nonempty);

}


info circularBuffer::obtain() {
    info data ;

    if (pthread_mutex_lock(&this->circular_buf_mtx))  /* Lock mutex */
        perror_exit("pthread_mutex_lock");

    while (this->isEmpty()) {
//        printf(">> Found Buffer Empty \n");
        pthread_cond_wait(&cond_nonempty, &this->circular_buf_mtx);
    }

    data = this->data[this->start];
    this->start = (this->start + 1) % this->buffSize;
    this->count--;

    if(pthread_mutex_unlock(&this->circular_buf_mtx))
        perror_exit("pthread_mutex_lock");

    pthread_cond_signal(&cond_nonfull);

//    cout << "obtain returns info: "<< data<<endl;

    return data;
}


bool circularBuffer::isFull() {
    return  this->count >= this->buffSize;
}

bool circularBuffer::isEmpty() {
    return this->count <= 0;
}

int circularBuffer::size() {
    return this->count;
}


bool info::operator==(const info &rhs) const {
    return ip == rhs.ip &&
           port == rhs.port &&
           pathName == rhs.pathName &&
           version == rhs.version;
}

bool info::operator!=(const info &rhs) const {
    return !(rhs == *this);
}

void info::prepareNewClient(clientsTuple tupl) {


    this->ip = tupl.ip;
    this->port = tupl.port;

    this->version = 0;
    this->pathName ="None";
}

info::info(uint32_t ip, uint16_t port, myString &pathName, unsigned int version) : ip(ip), port(port),
                                                                                   pathName(pathName),
                                                                                   version(version) {}


bool info::isNewClient() {
    return (this->pathName == "None" && this->version == 0);
}

bool info::isFilePath() {
    return (this->pathName != "None" && this->version == 0);
}

bool info::isFile() {
    return (this->pathName != "None" && this->version != 0);
}

bool info::isDir() {
    return (this->pathName != "None" && this->version == 1);
}

ostream &operator<<(ostream &os, const info &info1) {
    os << "ip: " << info1.ip << " port: " << info1.port << " pathName: " << info1.pathName << " version: "
       << info1.version;
    return os;
}

void info::setPathName(myString pathName) {
    info::pathName = pathName;
}

void info::setVersion(unsigned int version) {
    info::version = version;
}



