#ifndef __NRF24L01_DRV_H
#define __NRF24L01_DRV_H

#include "nRF24L01_Reg.h"
#include "nRF24L01_Obj.h"

#define NRF_POWER_ON(obj) nRF_OrWriteRegister(obj, NRF_REG_CFG, NRF_REG_CFG_PWR_UP_MSK)
#define NRF_POWER_DOWN(obj) nRF_AndWriteRegister(obj, NRF_REG_CFG, NRF_REG_CFG_PWR_UP_MSK)
#define NRF_RX_MODE(obj) nRF_OrWriteRegister(obj, NRF_REG_CFG, NRF_REG_CFG_PRIM_RX_MSK)
#define NRF_TX_MODE(obj) nRF_AndWriteRegister(obj, NRF_REG_CFG, ~NRF_REG_CFG_PRIM_RX_MSK)

void nRF_OrWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value );
void nRF_AndWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value );
void nRF_MaskWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value, uint8_t mask );
void nRF_ClearInterruptFlag( nRF_T* obj );
void nRF_Init( nRF_T* obj );
void nRF_SetAddressHeader_P0( nRF_T* obj, uint8_t RxAddressHeader[] );
void nRF_SetAddressHeader_P1_6( nRF_T* obj, uint8_t RxAddressHeader[] );
bool nRF_AddRxNode( nRF_T* obj, nRF_node_t* node, uint8_t ch );
void nRF_RemovalRxNode( nRF_T* obj, nRF_node_t* node, uint8_t ch );
bool nRF_TxPacket( nRF_T* obj, nRF_tx_packet_t* txPacket );
uint8_t nRF_RxPacket( nRF_T* obj );


void nRF_InterruptHookFunciation( nRF_T* obj );



#endif /* __NRF24L01_DRV_H */
