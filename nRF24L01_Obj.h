#ifndef __NRF24L01_OBJ_H
#define __NRF24L01_OBJ_H

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

//***********************************************************************
/**
 * @brief  介面鉤子函式結構
 * @note
 * @retval None
 */
typedef struct {
    //----------------------------------------------------------------
    /**
     * @brief  SPI 同步讀寫
     * @note
     * @param readBuf[] 讀陣列
     * @param writeBuf[] 寫陣列
     * @param size 資料大小
     * @retval void
     */
    void ( *ReadWrite )( uint8_t readBuf[], uint8_t writeBuf[], size_t size );

    //----------------------------------------------------------------
    /**
     * @brief  CE 腳位控制
     * @note
     * @param cePin 高態有效
     * @retval void
     */
    void ( *SetCE )( bool cePin );

    //----------------------------------------------------------------
    /**
     * @brief  延時函數
     * @note
     * @param us 單位微秒
     * @retval void
     */
    void ( *DelayUs )( uint32_t us );
    //----------------------------------------------------------------

} nRF_interface_T;

//***********************************************************************
/**
 * @brief 資料緩衝區結構
 * @note
 * @retval None
 */
typedef struct {
    uint8_t *data;    // 資料存放位址
    uint32_t length;  // 資料量
} nRF_buf_t;
//***********************************************************************
/**
 * @brief  節點結構
 * @note
 * @retval None
 */
typedef struct {
    // 需要初始化變量
    uint8_t    addr;                             // 節點位址
    uint8_t    payloadWide;                      // 有效負載量
    bool       autoAckEnabled : 1;               // 自動ACK功能
    bool       dynamicPayloadLengthEnabled : 1;  // 動態有效負載量，若為TRUE payloadWide 為無效值。
    nRF_buf_t *buf;                              // 緩衝區結構位址

    // 不需要初始化變量
    uint8_t ch;  // AddNode 後自動設動通道編號
} nRF_node_t;

//***********************************************************************
/**
 * @brief  傳送封包結構
 * @note
 * @retval None
 */
typedef struct {
    nRF_node_t *node;             // 目標節點結構
    uint8_t     addrHeader[ 4 ];  //目標位址標頭
    nRF_buf_t * txBuffer;         // 待傳送資料緩衝區結構
    bool        isAlter : 1;      // 設定是否更動
} nRF_tx_packet_t;

//***********************************************************************
/**
 * @brief  nRF 元件結構
 * @note
 * @retval None
 */
typedef struct {
    // 需要初始化
    nRF_interface_T *interface;             //介面結構
    nRF_node_t *     rxNode[ 5 ];           //接收節點結構
    uint8_t          RxAddressHeader[ 4 ];  // 接收地址標頭

    // 不需初始化
    nRF_tx_packet_t *txPacket;           // 傳送封包結構
    bool             interruptFlag : 1;  // 中斷旗標
} nRF_T;

#endif /* __NRF24L01_OBJ_H */
