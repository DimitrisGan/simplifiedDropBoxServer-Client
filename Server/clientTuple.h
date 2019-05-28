//
// Created by dimitrisgan on 28/5/2019.
//

#ifndef SERVER_CLIENTTUPLE_H
#define SERVER_CLIENTTUPLE_H

#include <cstdint>

struct clientsTuple{
    uint32_t ip;
    uint16_t port;

//    clientsTuple(uint32_t ip, uint16_t port);

    clientsTuple();

    bool operator==(const clientsTuple &rhs) const;

    bool operator!=(const clientsTuple &rhs) const;

};

#endif //SERVER_CLIENTTUPLE_H
