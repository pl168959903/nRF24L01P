
#include "nRF24L01_Base.h"

// 宏定義
//************************************************************************
#define CREATE_COMMAND_BUFFER( bufSize ) ( ( CommandBuffer_t* )malloc( bufSize + 1 ) )

// 結構式
//************************************************************************
/**
 * @brief  NRF SPI協議封包結構
 * @note
 * @retval None
 */
typedef struct CommandBuffer {
    union {
        uint8_t command;  // 指令(SPI傳送)
        nRF_statusReg_t status;   // 狀態(SPI接收)
    };
    uint8_t buffer[0];  // 緩衝區起始位址
} CommandBuffer_t;

// 基礎函式
//************************************************************************
/**
 * @brief  建立指令緩衝區
 * @note
 * @param  cmd: 指令
 * @param  size: 緩衝區大小，不包含指令。
 * @retval 緩衝區結構位址
 */
static CommandBuffer_t* CommandBuffer_create( uint8_t cmd, uint8_t size ) {
    CommandBuffer_t* retval = CREATE_COMMAND_BUFFER( size );
    retval->command         = cmd;
    return retval;
}

//************************************************************************
/**
 * @brief  SPI 指令寫入
 * @note   
 * @param  nrf: 目標物件
 * @param  command: 指令
 * @param  array: 寫入資料陣列
 * @param  length: 操作長度
 * @retval 狀態暫存器
 */
nRF_statusReg_t nRF_SpiCmdWrite( nRF_T* nrf, uint8_t command, uint8_t* array, uint8_t length ) {
    CommandBuffer_t* cmdBuf = CommandBuffer_create( command, length );
    if ( length > 0 ) memcpy( cmdBuf->buffer, array, length );
    nrf->interface->ReadWrite( ( uint8_t* )cmdBuf, ( uint8_t* )cmdBuf, length + 1 );
    nrf->statusRegister = cmdBuf->status;
    free( cmdBuf );
    return nrf->statusRegister;
}

//************************************************************************
/**
 * @brief  SPI 指令讀取
 * @note   
 * @param  nrf: 目標物件
 * @param  command: 指令
 * @param  array: 賭取儲存陣列
 * @param  length: 操作長度
 * @retval 狀態暫存器
 */
nRF_statusReg_t nRF_SpiCmdRead( nRF_T* nrf, uint8_t command, uint8_t* array, uint8_t length ) {
    CommandBuffer_t* cmdBuf = CommandBuffer_create( command, length );
    nrf->interface->ReadWrite( ( uint8_t* )cmdBuf, ( uint8_t* )cmdBuf, length + 1 );
    nrf->statusRegister = cmdBuf->status;
    if ( length > 0 ) memcpy( array, cmdBuf->buffer, length );
    free( cmdBuf );
    return nrf->statusRegister;
}
