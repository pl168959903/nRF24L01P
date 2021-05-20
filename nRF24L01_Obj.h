#ifndef __NRF24L01_OBJ_H
#define __NRF24L01_OBJ_H

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

typedef struct {
    void ( *ReadWrite )( uint8_t readBuf[], uint8_t writeBuf[], size_t size );
    void (*SetCE)(bool cePin);
} nRF_interface_T;

typedef struct {
    nRF_interface_T *interface;
}nRF_T;


#endif /* __NRF24L01_OBJ_H */
