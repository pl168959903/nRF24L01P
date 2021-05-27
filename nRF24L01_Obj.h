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
    uint8_t payloadWide;
    bool    autoAckEnabled : 1;
    bool    dynamicPayloadLengthEnabled : 1; 
    uint8_t ch;
} nRF_node_t;

typedef struct {
    nRF_node_t * node;
    uint8_t addrHeader[4];
    uint8_t *data;
    uint8_t dataLength;
} nRF_tx_packet_t;

typedef struct {
    nRF_interface_T *interface;
    nRF_node_t *  rxNode[ 5 ];
    nRF_tx_packet_t * txPacket;
    bool interruptFlag : 1;
} nRF_T;

#endif /* __NRF24L01_OBJ_H */
