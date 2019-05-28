//
// Created by dimitrisgan on 28/5/2019.
//

#include "clientTuple.h"


//clientsTuple::clientsTuple(uint32_t ip, uint16_t port) : ip(ip), port(port) {}

clientsTuple::clientsTuple() {
    ip = 0;
    port=0;
}


bool clientsTuple::operator==(const clientsTuple &rhs) const {
    return ip == rhs.ip &&
           port == rhs.port;
}

bool clientsTuple::operator!=(const clientsTuple &rhs) const {
    return !(rhs == *this);
}
