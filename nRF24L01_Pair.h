#ifndef __NRF24L01_PAIR_H
#define __NRF24L01_PAIR_H

#include <stdint.h>

typedef struct {
    uint8_t PairAddress[5];
    uint8_t token;
} nRF_pair_t;

#endif /* __NRF24L01_PAIR_H */
