#include <stdlib.h>
#include <string.h>

#include "nRF24L01_Obj.h"
#include "nRF24L01_Reg.h"

/**
 * @brief  連續讀取多個字元組
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  reg: 讀取暫存器起始位置
 * @param  array[]: 輸出陣列位置
 * @param  size: 讀取大小
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_ReadRegArray( nRF_T* nrf, uint8_t reg, uint8_t array[], size_t size ) {
    uint8_t nRF_Status;
    size_t  dataSize = size + 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = ( uint8_t )( ( reg | NRF_CMD_R_REGISTER ) & NRF_NRF_CMD_MASK );

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];
    memcpy( array, &( data[ 1 ] ), size );

    free( data );
    return nRF_Status;
}

/**
 * @brief  連續寫入多個字元組
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  reg: 寫入暫存器起始位置
 * @param  array[]: 寫入資料陣列
 * @param  size: 寫入字元組大小
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_WriteRegArray( nRF_T* nrf, uint8_t reg, uint8_t array[], size_t size ) {
    uint8_t nRF_Status;
    size_t  dataSize = size + 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = ( uint8_t )( ( reg | NRF_CMD_W_REGISTER ) & NRF_NRF_CMD_MASK );
    memcpy( &( data[ 1 ] ), array, size );

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return nRF_Status;
}

/**
 * @brief  讀取一個字元組
 * @note   
 * @param  nrf: nRF物件結構 nRF物件結構
 * @param  reg: 讀取暫存器位置
 * @param  data: 輸出位址
 * @retval nRF 狀態暫存器 nRF 狀態暫存器
 */
uint8_t nRF_ReadRegByte( nRF_T* nrf, uint8_t reg, uint8_t* data ) {
    return nRF_ReadRegArray( nrf, reg, data, 1 );
}

/**
 * @brief  寫入一個字元組
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  reg: 寫入暫存器位置
 * @param  data: 寫入資料
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_WriteRegByte( nRF_T* nrf, uint8_t reg, uint8_t data ) {
    return nRF_WriteRegArray( nrf, reg, ( uint8_t[] ){ data }, 1 );
}

/**
 * @brief  讀取有效附載
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  array[]: 輸出陣列位置
 * @param  size: 讀取大小
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_RxPayload( nRF_T* nrf, uint8_t array[], size_t size ) {
    uint8_t nRF_Status;
    size_t  dataSize = size + 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_R_RX_PAYLOAD;

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];
    memcpy( array, &( data[ 1 ] ), size );

    free( data );
    return nRF_Status;
}

/**
 * @brief  寫入有效附載
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  array[]: 寫入資料陣列
 * @param  size: 寫入字元組大小
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_TxPayload( nRF_T* nrf, uint8_t array[], size_t size ) {
    uint8_t nRF_Status;
    size_t  dataSize = size + 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_W_TX_PAYLOAD;
    memcpy( &( data[ 1 ] ), array, size );

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return nRF_Status;
}

/**
 * @brief  清空TX有效附載
 * @note   
 * @param  nrf: nRF物件結構 
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_FlushTx( nRF_T* nrf ) {
    uint8_t nRF_Status;
    size_t  dataSize = 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_FLUSH_TX;

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];
    free( data );

    return nRF_Status;
}

/**
 * @brief  清空RX有效附載
 * @note   
 * @param  nrf: nRF物件結構 
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_FlushRx( nRF_T* nrf ) {
    uint8_t nRF_Status;
    size_t  dataSize = 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_FLUSH_RX;

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return nRF_Status;
}

/**
 * @brief  重發TX有效附載
 * @note   
 * @param  nrf: nRF物件結構 
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_ReuseTx( nRF_T* nrf ) {
    uint8_t nRF_Status;
    size_t  dataSize = 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_REUSE_TX_PL;

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return nRF_Status;
}

/**
 * @brief  讀取RX有效附載資料長度
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  payloadWide: 
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_ReadRxPayloadWide( nRF_T* nrf, uint8_t* payloadWide ) {
    uint8_t nRF_Status;
    size_t  dataSize = 2;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_R_RX_PL_WID;

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status   = data[ 0 ];
    *payloadWide = data[ 1 ];

    free( data );
    return data[ 0 ];
}

/**
 * @brief  寫入回應有效附載
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  array[]: 寫入資料陣列
 * @param  size: 寫入字元組大小
 * @param  ch: 回應通道
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_AckPayload( nRF_T* nrf, uint8_t array[], size_t size, uint8_t ch ) {
    uint8_t nRF_Status;
    size_t  dataSize = size + 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = ( uint8_t )( NRF_CMD_W_ACK_PAYLOAD | ( ch & 0x7 ) );
    memcpy( &( data[ 1 ] ), array, size );

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return data[ 0 ];
}

/**
 * @brief  寫入有效附載不使用回應
 * @note   
 * @param  nrf: nRF物件結構 
 * @param  array[]: 寫入資料陣列
 * @param  size: 寫入字元組大小
 * @retval nRF 狀態暫存器 
 */
uint8_t nRF_TxWithoutAutoAck( nRF_T* nrf, uint8_t array[], size_t size ) {
    uint8_t nRF_Status;
    size_t  dataSize = size + 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_W_TX_PAYLOAD_NOACK;
    memcpy( &( data[ 1 ] ), array, size );

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return data[ 0 ];
}

/**
 * @brief  空操作
 * @note   
 * @param  nrf: nRF物件結構 
 * @retval nRF 狀態暫存器 
 */
uint8_t NRF_Nop( nRF_T* nrf ) {
    uint8_t nRF_Status;
    size_t  dataSize = 1;

    uint8_t* data = malloc( dataSize );
    data[ 0 ]     = NRF_CMD_NOP;

    nrf->interface->ReadWrite( data, data, dataSize );

    nRF_Status = data[ 0 ];

    free( data );
    return nRF_Status;
}
