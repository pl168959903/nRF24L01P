#ifndef __NRF24L01_OBJ_H
#define __NRF24L01_OBJ_H

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

typedef struct {
    void ( *ReadWrite )( uint8_t readBuf[], uint8_t writeBuf[], size_t size );
    void ( *SetCE )( bool cePin );
    void ( *DelayUs )( uint32_t us );
} nRF_interface_T;

typedef struct {
    uint8_t addr;
    bool    isAutoAck : 1;
    bool    isDynamicPayloadLength : 1;
    uint8_t ch;
} nRF_rx_node_t;

typedef struct {
    nRF_interface_T *interface;
    nRF_rx_node_t *  node_t[ 5 ];
} nRF_T;

#endif /* __NRF24L01_OBJ_H */
