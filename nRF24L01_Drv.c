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
    nRF_WriteRegByte( obj, NRF_REG_EN_AA, 0x3F );
    nRF_WriteRegByte( obj, NRF_REG_EN_RXADDR, 0x03 );
    nRF_WriteRegByte( obj, NRF_REG_SETUP_AW, 0x03 );
    nRF_WriteRegByte( obj, NRF_REG_SETUP_RETR, 0x03 );
    nRF_WriteRegByte( obj, NRF_REG_RF_CH, 0x02 );
    nRF_WriteRegByte( obj, NRF_REG_RF_SETUP, 0x0F );
    nRF_WriteRegByte( obj, NRF_REG_OBSERVE_TX, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_CD, 0x00 );
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P0, ( uint8_t[] ){ 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 }, 5 );
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P1, ( uint8_t[] ){ 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 }, 5 );
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P2, 0xC3 );
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P3, 0xC4 );
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P4, 0xC5 );
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P5, 0xC6 );
    nRF_WriteRegArray( obj, NRF_REG_TX_ADDR, ( uint8_t[] ){ 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 }, 5 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P0, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P1, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P2, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P3, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P4, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P5, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_FIFO_STATUS, 0x11 );
    nRF_WriteRegByte( obj, NRF_REG_DYNPD, 0x00 );
    nRF_WriteRegByte( obj, NRF_REG_FEATURE, 0x00 );
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
        nRF_OrWriteRegister( obj, ( uint8_t )( NRF_REG_RX_PW_P1 + node->ch ), node->payloadWide );
    }

    return true;
}

void nRF_TxSendPacket( nRF_T* obj, nRF_tx_packet_t* txPacket ) {
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
            nRF_AndWriteRegister( obj, NRF_REG_DYNPD, 0x1 << 0 );                    // 關閉P0動態負載長度
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

    while(true){
        
    }
}

void nRF_InterruptHookFunciation(nRF_T* obj){
    obj->interruptFlag = true;
}
