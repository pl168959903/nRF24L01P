#ifndef __NRF24L01_BASE_H
#define __NRF24L01_BASE_H

#include "string.h"

#include "nRF24L01_Obj.h"
#include "nRF24L01_Reg.h"

// Function
//************************************************************************
/**
 * @brief  SPI協議封包傳送
 * @note
 * @param  nrf: nRF物件結構
 * @param  command: 指令
 * @param  array: 傳送資料起始位址
 * @param  length: 資料長度
 * @retval NRF狀態暫存器
 */
nRF_statusReg_t nRF_SpiCmdWrite( nRF_T* nrf, uint8_t command, uint8_t* array, uint8_t length );

/**
 * @brief  SPI協議封包接收
 * @note
 * @param  nrf: nRF物件結構
 * @param  command: 指令
 * @param  array: 接收資料起始位址
 * @param  length: 資料長度
 * @retval NRF狀態暫存器
 */
nRF_statusReg_t nRF_SpiCmdRead( nRF_T* nrf, uint8_t command, uint8_t* array, uint8_t length );

// 宏函式
//************************************************************************
/**
 * @brief  連續讀取多個字元組
 * @note
 * @param  nrf: nRF物件結構
 * @param  reg: 讀取暫存器起始位置
 * @param  array: 輸出陣列位置
 * @param  length: 讀取長度
 * @retval nRF 狀態暫存器
 */
#define NRF_READ_REG_ARRAY( nrf, reg, array, length ) nRF_SpiCmdRead( nrf, ( NRF_CMD_R_REGISTER | ( reg & NRF_NRF_CMD_MASK ) ), ( uint8_t* )array, length )

/**
 * @brief  連續寫入多個字元組
 * @note
 * @param  nrf: nRF物件結構
 * @param  reg: 寫入暫存器起始位置
 * @param  array: 寫入資料陣列
 * @param  length: 寫入長度
 * @retval nRF 狀態暫存器
 */
#define NRF_WRITE_REG_ARRAY( nrf, reg, array, length ) nRF_SpiCmdWrite( nrf, ( NRF_CMD_W_REGISTER | ( reg & NRF_NRF_CMD_MASK ) ), ( uint8_t* )array, length )

/**
 * @brief  讀取一個字元組
 * @note
 * @param  nrf: nRF物件結構 nRF物件結構
 * @param  reg: 讀取暫存器位置
 * @param  dataByteAddress: 輸出位址
 * @retval nRF 狀態暫存器 nRF 狀態暫存器
 */
#define NRF_READ_REG_BYTE( nrf, reg, dataByteAddress ) NRF_READ_REG_ARRAY( nrf, reg, dataByteAddress, 1 )

/**
 * @brief  寫入一個字元組
 * @note
 * @param  nrf: nRF物件結構
 * @param  reg: 寫入暫存器位置
 * @param  dataByte: 寫入資料
 * @retval nRF 狀態暫存器
 */
#define NRF_WRITE_REG_BYTE( nrf, reg, dataByte ) NRF_WRITE_REG_ARRAY( nrf, reg, ( uint8_t[] ){ dataByte }, 1 )

/**
 * @brief  讀取有效附載
 * @note
 * @param  nrf: nRF物件結構
 * @param  array: 輸入位址
 * @param  length: 讀取大小
 * @retval nRF 狀態暫存器
 */
#define NRF_RX_PAYLOAD( nrf, array, length ) nRF_SpiCmdRead( nrf, NRF_CMD_RX_PAYLOAD, ( uint8_t* )array, length )

/**
 * @brief  寫入有效附載
 * @note
 * @param  nrf: nRF物件結構
 * @param  array: 輸出位址
 * @param  length: 寫入字元組大小
 * @retval nRF 狀態暫存器
 */
#define NRF_TX_PAYLOAD( nrf, array, length ) nRF_SpiCmdWrite( nrf, NRF_CMD_TX_PAYLOAD, ( uint8_t* )array, length )

/**
 * @brief  清空TX有效附載
 * @note
 * @param  nrf: nRF物件結構
 * @retval nRF 狀態暫存器
 */
#define NRF_FLUSH_TX( nrf ) nRF_SpiCmdRead( nrf, NRF_CMD_FLUSH_TX, NULL, 0 )

/**
 * @brief  清空RX有效附載
 * @note
 * @param  nrf: nRF物件結構
 * @retval nRF 狀態暫存器
 */
#define NRF_FLUSH_RX( nrf ) nRF_SpiCmdRead( nrf, NRF_CMD_FLUSH_RX, NULL, 0 )

/**
 * @brief  重發TX有效附載
 * @note
 * @param  nrf: nRF物件結構
 * @retval nRF 狀態暫存器
 */
#define NRF_REUSE_TX( nrf ) nRF_SpiCmdRead( nrf, NRF_CMD_REUSE_TX_PL, NULL, 0 )

/**
 * @brief  讀取RX有效附載資料長度
 * @note
 * @param  nrf: nRF物件結構
 * @param  payloadWide: 資料長度
 * @retval nRF 狀態暫存器
 */
#define NRF_READ_RX_PAYLOAD_WIDE( nrf, payloadWide ) nRF_SpiCmdRead( nrf, NRF_CMD_R_RX_PL_WID, ( uint8_t* )payloadWide, 1 )

/**
 * @brief  寫入回應有效附載
 * @note
 * @param  nrf: nRF物件結構
 * @param  array: 寫入資料陣列
 * @param  length: 寫入字元組大小
 * @param  channel: 回應通道
 * @retval nRF 狀態暫存器
 */
#define NRF_TX_ACK_PAYLOAD( nrf, array, length, channel ) nRF_SpiCmdWrite( nrf, NRF_CMD_W_ACK_PAYLOAD | ( channel & NRF_CMD_W_ACK_PAYLOAD_CH_MASK ), ( uint8_t* )array, length )

/**
 * @brief  寫入有效附載不使用回應
 * @note
 * @param  nrf: nRF物件結構
 * @param  array[]: 寫入資料陣列
 * @param  size: 寫入字元組大小
 * @retval nRF 狀態暫存器
 */
#define NRF_TX_WITHOUT_AUTO_ACK( nrf, array, length ) nRF_SpiCmdWrite( nrf, NRF_CMD_W_TX_PAYLOAD_NOACK, ( uint8_t* )array, length )

/**
 * @brief  空操作
 * @note
 * @param  nrf: nRF物件結構
 * @retval nRF 狀態暫存器
 */
#define NRF_NOP( nrf ) nRF_SpiCmdRead( nrf, NRF_CMD_NOP, NULL, 0 )

#endif /* __NRF24L01_BASE_H */
