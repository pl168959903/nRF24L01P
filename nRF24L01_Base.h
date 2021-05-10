#ifndef __NRF24L01_BASE_H
#define __NRF24L01_BASE_H


#include <stdint.h>
#include <string.h>
#include "nRF24L01.h"

uint8_t nRF_ReadRegByte( nRF_T* nrf, uint8_t reg, uint8_t *data );
uint8_t nRF_WriteRegByte( nRF_T* nrf, uint8_t reg, uint8_t data );
uint8_t nRF_ReadRegArray( nRF_T* nrf, uint8_t reg, uint8_t array[], size_t size );
uint8_t nRF_WriteRegArray( nRF_T* nrf, uint8_t reg, uint8_t array[], size_t size );
uint8_t nRF_RxPayload( nRF_T* nrf, uint8_t array[], size_t size );
uint8_t nRF_TxPayload( nRF_T* nrf, uint8_t array[], size_t size );
uint8_t nRF_FlushTx( nRF_T* nrf );
uint8_t nRF_FlushRx( nRF_T* nrf );
uint8_t nRF_ReuseTx( nRF_T* nrf );
uint8_t nRF_ReadRxPayloadWide( nRF_T* nrf, uint8_t* payloadWide );
uint8_t nRF_AckPayload( nRF_T* nrf, uint8_t array[], size_t size, uint8_t ch );
uint8_t nRF_TxWithoutAutoAck( nRF_T* nrf, uint8_t array[], size_t size );
uint8_t NRF_Nop( nRF_T* nrf );

#endif /* __NRF24L01_BASE_H */
