#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "nRF24L01_Base_API.h"
#include "nRF24L01_Config.h"
#include "nRF24L01_Obj.h"
#include "nRF24L01_Reg.h"

void nRF_OrWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value ) {
    uint8_t regData;
    nRF_ReadRegByte( obj, reg, &regData );
    regData |= value;
    nRF_WriteRegByte( obj, reg, regData );
}

void nRF_AndWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value ) {
    uint8_t regData;
    nRF_ReadRegByte( obj, reg, &regData );
    regData &= value;
    nRF_WriteRegByte( obj, reg, regData );
}

void nRF_ClearInterruptFlag( nRF_T* obj ) {
    uint8_t flags = nRF_Nop( obj );
    nRF_WriteRegByte( obj, NRF_REG_STATUS, flags );
}

void nRF_Init( nRF_T* obj ) {
    obj->interface->DelayUs( _NRF_POWER_ON_RESET_US );

    obj->interface->SetCE( false );
    nRF_FlushTx( obj );
    nRF_FlushRx( obj );
    nRF_ClearInterruptFlag( obj );

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
}

void nRF_SetAddressHeader( nRF_T* obj, uint8_t addrHeader[] ) {
    uint8_t data[ 5 ];
    memcpy( &data[ 1 ], addrHeader, 4 );
    nRF_WriteRegArray( obj, NRF_REG_TX_ADDR, data, 5 );
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P1, data, 5 );
}

bool nRF_AddRxNode( nRF_T* obj, nRF_node_t* node ) {
    // 尋找空通道
    node->ch = NULL;
    for ( uint8_t i = 0; i < 5; i++ ) {
        if ( obj->rxNode[ i ] == NULL ) {
            node->ch         = i;
            obj->rxNode[ i ] = node;
        }
    }

    // 通道建立失敗
    if ( node->ch == NULL ) return false;

    // 寫入接收位址
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P1 + node->ch, node->addr );

    // 啟用通道
    nRF_OrWriteRegister( obj, NRF_REG_EN_RXADDR, 0x1 << node->ch );

    // 設定自動ACK功能
    if ( ( node->autoAckEnabled | node->dynamicPayloadLengthEnabled ) == true ) {
        nRF_OrWriteRegister( obj, NRF_REG_EN_AA, 0x1 << node->ch );  //
    }

    // 設定 動態;靜態 有效負載長度
    if ( node->dynamicPayloadLengthEnabled == true ) {
        nRF_OrWriteRegister( obj, NRF_REG_DYNPD, 0x1 << node->ch );  //
    }
    else {
        nRF_WriteRegByte( obj, ( uint8_t )( NRF_REG_RX_PW_P1 + node->ch ), node->payloadWide );
    }

    return true;
}

void nRF_RemovalRxNode( nRF_T* obj, nRF_node_t* node ) {

    nRF_AndWriteRegister( obj, NRF_REG_EN_RXADDR, ~( 0x1 << node->ch ) );
    nRF_AndWriteRegister( obj, NRF_REG_EN_AA, ~( 0x1 << node->ch ) );
    nRF_AndWriteRegister( obj, NRF_REG_DYNPD, ~( 0x1 << node->ch ) );
    nRF_WriteRegByte( obj, ( uint8_t )( NRF_REG_RX_PW_P1 + node->ch ), 0x00 );

    for ( uint8_t i = 0; i < 5; i++ ) {
        if ( obj->rxNode[ i ] == node ) {
            node->ch         = NULL;
            obj->rxNode[ i ] = NULL;
        }
    }
}

bool nRF_TxPacket( nRF_T* obj, nRF_tx_packet_t* txPacket ) {

    uint8_t retval;

    // 檢查傳送設定是否與上次相同
    if ( obj->txPacket != txPacket ) {

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
    }

    // 動態負載長度傳送
    if ( txPacket->node->dynamicPayloadLengthEnabled == true ) {
        nRF_TxPayload( obj, txPacket->data, ( ( txPacket->dataLength > 32 ) ? 32 : txPacket->dataLength ) );  //
    }
    // 靜態負載長度傳送
    else {
        uint8_t* sendData = malloc( txPacket->node->payloadWide );
        memcpy( sendData, txPacket->data, txPacket->node->payloadWide );
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
