#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "nRF24L01_Base_API.h"
#include "nRF24L01_Config.h"
#include "nRF24L01_Obj.h"
#include "nRF24L01_Reg.h"
//***********************************************************************
/**
 * @brief  暫存器或寫操作
 * @note
 * @param  obj: 目標物件
 * @param  reg: 暫存器
 * @param  value: 操作資料
 * @retval None
 */
void nRF_OrWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value ) {
    uint8_t regData;
    nRF_ReadRegByte( obj, reg, &regData );
    regData |= value;
    nRF_WriteRegByte( obj, reg, regData );
}

//***********************************************************************
/**
 * @brief  暫存器及寫操作
 * @note
 * @param  obj: 目標物件
 * @param  reg: 暫存器
 * @param  value: 操作資料
 * @retval None
 */
void nRF_AndWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value ) {
    uint8_t regData;
    nRF_ReadRegByte( obj, reg, &regData );
    regData &= value;
    nRF_WriteRegByte( obj, reg, regData );
}

//***********************************************************************
/**
 * @brief  暫存器遮罩寫操作
 * @note
 * @param  obj: 目標物件
 * @param  reg: 暫存器
 * @param  value: 操作資料
 * @param  mask: 資料遮罩
 * @retval None
 */
void nRF_MaskWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value, uint8_t mask ) {
    uint8_t regData;
    nRF_ReadRegByte( obj, reg, &regData );
    regData = ( regData & mask ) | ( value & ~mask );
    nRF_WriteRegByte( obj, reg, regData );
}

//***********************************************************************
/**
 * @brief  清除中斷
 * @note
 * @param  obj: 目標物件
 * @retval None
 */
void nRF_ClearInterruptFlag( nRF_T* obj ) {
    uint8_t flags = nRF_Nop( obj );
    nRF_WriteRegByte( obj, NRF_REG_STATUS, flags );
}

//***********************************************************************
/**
 * @brief  初始化物件暫存器
 * @note
 * @param  obj: 目標物件
 * @retval None
 */
void nRF_Init( nRF_T* obj ) {
    obj->interface->DelayUs( _NRF_POWER_ON_RESET_US );  // 上電延時開機
    obj->interface->SetCE( false );
    nRF_FlushTx( obj );             // 清除緩衝區
    nRF_FlushRx( obj );             // 清除緩衝區
    nRF_ClearInterruptFlag( obj );  // 清除中斷

    // 初始化暫存器
    nRF_WriteRegByte( obj, NRF_REG_CFG, 0x08 );
    nRF_WriteRegByte( obj, NRF_REG_EN_AA, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_EN_RXADDR, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_SETUP_AW, 0x03 );
    nRF_WriteRegByte( obj, NRF_REG_SETUP_RETR, 0xFF );
    nRF_WriteRegByte( obj, NRF_REG_RF_CH, 0x02 );
    nRF_WriteRegByte( obj, NRF_REG_RF_SETUP, 0x0F );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P0, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P1, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P2, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P3, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P4, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P5, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_DYNPD, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_FEATURE, 0x07 );

    // 初始化接收位址標頭
    nRF_SetAddressHeader_P0( obj, obj->RxAddressHeader_0 );
    nRF_SetAddressHeader_P1_6( obj, obj->RxAddressHeader_1_6 );
}

//***********************************************************************
/**
 * @brief 設定P0接收位址標頭
 * @note
 * @param  obj: 目標物件
 * @retval None
 */
void nRF_SetAddressHeader_P0( nRF_T* obj, uint8_t RxAddressHeader[] ) {
    uint8_t Address[ 5 ] = { 0 };
    memcpy( obj->RxAddressHeader_0, RxAddressHeader, 4 );
    memcpy( &Address[ 1 ], RxAddressHeader, 4 );
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P0, Address, 5 );
}

//***********************************************************************
/**
 * @brief 設定P1~P6接收位址標頭
 * @note
 * @param  obj: 目標物件
 * @retval None
 */
void nRF_SetAddressHeader_P1_6( nRF_T* obj, uint8_t RxAddressHeader[] ) {
    uint8_t Address[ 5 ] = { 0 };
    memcpy( obj->RxAddressHeader_1_6, RxAddressHeader, 4 );
    memcpy( &Address[ 1 ], RxAddressHeader, 4 );
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P1, Address, 5 );
}

//***********************************************************************
/**
 * @brief  新增通道
 * @note
 * @param  obj: 目標物件
 * @param  node:  新增的節點結構
 * @retval
 */
bool nRF_AddRxNode( nRF_T* obj, nRF_node_t* node, uint8_t ch ) {

    obj->rxNode[ch] = node;

    // 寫入接收位址
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P0 + ch, node->addr );

    // 啟用通道
    nRF_OrWriteRegister( obj, NRF_REG_EN_RXADDR, 0x1 << ch );

    // 設定自動ACK功能
    if ( ( node->autoAckEnabled | node->dynamicPayloadLengthEnabled ) == true ) {
        nRF_OrWriteRegister( obj, NRF_REG_EN_AA, 0x1 << ch );  //
    }
    else {
        nRF_AndWriteRegister( obj, NRF_REG_EN_AA, ~( 0x1 << ch ) );
    }

    // 設定 動態;靜態 有效負載長度
    if ( node->dynamicPayloadLengthEnabled == true ) {
        nRF_OrWriteRegister( obj, NRF_REG_DYNPD, 0x1 << ch );  //
    }
    else {
        nRF_AndWriteRegister( obj, NRF_REG_DYNPD, ~( 0x1 << ch ) );
        nRF_WriteRegByte( obj, ( uint8_t )( NRF_REG_RX_PW_P0 + ch ), node->payloadWide );  // 設定靜態長度
    }

    return true;
}

//***********************************************************************
/**
 * @brief  移除通道
 * @note
 * @param  obj: 目標物件
 * @param  node: 移除的節點結構
 * @retval None
 */
void nRF_RemovalRxNode( nRF_T* obj, nRF_node_t* node ) {
    nRF_AndWriteRegister( obj, NRF_REG_EN_RXADDR, ~( 0x1 << node->ch ) );
}

//***********************************************************************
/**
 * @brief  傳送封包
 * @note
 * @param  obj: 目標物件
 * @param  txPacket: 傳送封包結構
 * @retval 結果
 */
bool nRF_TxPacket( nRF_T* obj, nRF_tx_packet_t* txPacket ) {

    uint8_t retval;

    // 檢查傳送設定是否與上次相同
    if ( obj->txPacket != txPacket || txPacket->isAlter == true ) {

        // 設定 TX地址
        uint8_t DestAddress[ 5 ];
        memcpy( &DestAddress[ 1 ], txPacket->addrHeader, 4 );
        DestAddress[ 0 ] = txPacket->node->addr;
        nRF_WriteRegArray( obj, NRF_REG_TX_ADDR, DestAddress, 5 );

        // RX P0相關設定
        if ( ( txPacket->node->autoAckEnabled | txPacket->node->dynamicPayloadLengthEnabled ) == true ) {
            nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P0, DestAddress, 5 );  //地址
            nRF_OrWriteRegister( obj, NRF_REG_EN_RXADDR, 0x1 << 0 );       //通道啟用
            nRF_OrWriteRegister( obj, NRF_REG_EN_AA, 0x1 << 0 );           // 自動ACK
        }

        //動態負載長度設定
        if ( txPacket->node->dynamicPayloadLengthEnabled ) {
            nRF_OrWriteRegister( obj, NRF_REG_DYNPD, 0x1 << 0 );  // 開啟P0動態負載長度
        }
        else {
            nRF_AndWriteRegister( obj, NRF_REG_DYNPD, ~( 0x1 << 0 ) );               // 關閉P0動態負載長度
            nRF_WriteRegByte( obj, NRF_REG_RX_PW_P0, txPacket->node->payloadWide );  // 設定P0靜態負載長度
        }

        txPacket->isAlter = false;
        obj->txPacket     = txPacket;
    }

    // 動態負載長度傳送
    if ( txPacket->node->dynamicPayloadLengthEnabled == true ) {
        nRF_TxPayload( obj, txPacket->txBuffer->data, ( ( txPacket->txBuffer->length > 32 ) ? 32 : txPacket->txBuffer->length ) );  //
    }
    // 靜態負載長度傳送
    else {
        uint8_t* sendData = malloc( txPacket->node->payloadWide );
        memcpy( sendData, txPacket->txBuffer->data, txPacket->node->payloadWide );
        nRF_TxPayload( obj, sendData, txPacket->node->payloadWide );
        free( sendData );
    }

    // 等待中斷信號
    while ( obj->interruptFlag == false ) {};

    // 取得中斷狀態
    uint8_t interruptFlag = nRF_Nop( obj );

    // 傳輸成功
    if ( interruptFlag & NRF_REG_STATUS_TX_DS_MSK ) {
        // 傳輸成功並且ACK封包有RX負載
        if ( interruptFlag & NRF_REG_STATUS_RX_DR_MSK ) {
            if ( obj->rxNode[ 0 ] != NULL &&             //
                 obj->rxNode[ 0 ]->buf != NULL &&        //
                 obj->rxNode[ 0 ]->buf->data != NULL &&  //
                 ( interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK ) == 0x0 ) {
                uint8_t data_length;
                nRF_ReadRxPayloadWide( obj, &data_length );
                nRF_RxPayload( obj, obj->rxNode[ 0 ]->buf->data, data_length );
                obj->rxNode[ 0 ]->buf->length = data_length;
            }
            else {
                nRF_FlushRx( obj );
            }
        }

        // 清除中斷旗標
        nRF_WriteRegByte( obj, NRF_REG_STATUS, interruptFlag );
        obj->interruptFlag = false;

        retval = true;
    }
    // 傳輸操作超時
    else if ( interruptFlag & NRF_REG_STATUS_MAX_RT_MSK ) {
        retval = false;
    }

    return retval;
}

//***********************************************************************
/**
 * @brief  接收封包
 * @note
 * @param  obj: 目標物件
 * @retval 接收的通道編號
 */
uint8_t nRF_RxPacket( nRF_T* obj ) {
    uint8_t interruptFlag = nRF_Nop( obj );
    uint8_t data_length;
    nRF_ReadRxPayloadWide( obj, &data_length );

    if ( obj->rxNode[ 0 ] != NULL &&       //
         obj->rxNode[ 0 ]->buf != NULL &&  //
         obj->rxNode[ 0 ]->buf->data != NULL ) {
        nRF_RxPayload( obj, obj->rxNode[ interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK ]->buf->data, data_length );
        obj->rxNode[ interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK ]->buf->length = data_length;
    }
    else {
        nRF_FlushRx( obj );
    }

    // 清除中斷旗標
    nRF_WriteRegByte( obj, NRF_REG_STATUS, interruptFlag );
    obj->interruptFlag = false;

    return interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK;
}

void nRF_InterruptHookFunciation( nRF_T* obj ) {
    obj->interruptFlag = true;
}
