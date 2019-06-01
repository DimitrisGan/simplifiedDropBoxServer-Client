//
// Created by dimitrisgan on 28/5/2019.
//

#ifndef SERVER_CLIENTTUPLE_H
#define SERVER_CLIENTTUPLE_H

#include <cstdint>
#include <ostream>

struct clientsTuple{
    uint32_t ip;
    uint16_t port;

    clientsTuple(uint32_t ip, uint16_t port);

    clientsTuple();

    bool operator==(const clientsTuple &rhs) const;

    bool operator!=(const clientsTuple &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const clientsTuple &tuple);

};

#endif //SERVER_CLIENTTUPLE_H
