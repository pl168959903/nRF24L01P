#include "nRF24L01_Drv.h"

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
    NRF_READ_REG_BYTE( obj, reg, &regData );
    regData |= value;
    NRF_WRITE_REG_BYTE( obj, reg, regData );
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
    NRF_READ_REG_BYTE( obj, reg, &regData );
    regData &= value;
    NRF_WRITE_REG_BYTE( obj, reg, regData );
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
    NRF_READ_REG_BYTE( obj, reg, &regData );
    regData = ( regData & mask ) | ( value & ~mask );
    NRF_WRITE_REG_BYTE( obj, reg, regData );
}

//***********************************************************************
/**
 * @brief  清除中斷
 * @note
 * @param  obj: 目標物件
 * @retval None
 */
void nRF_ClearInterruptFlag( nRF_T* obj ) {
    nRF_statusReg_t flags = NRF_NOP( obj );
    NRF_WRITE_REG_BYTE( obj, NRF_REG_STATUS, *( uint8_t* )&flags );
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
    NRF_FLUSH_TX( obj );          // 清除緩衝區
    NRF_FLUSH_RX( obj );          // 清除緩衝區
    NRF_CLEAR_STATUS_REG( obj );  // 清除中斷

    uint8_t regInitData[] = {
#include "nRF_RegInit.txt"
    };

    // 初始化暫存器
    for ( size_t i = 0; i < sizeof( regInitData ); i += 2 ) { NRF_WRITE_REG_BYTE( obj, regInitData[ i ], regInitData[ i + 1 ] ); }
}

//***********************************************************************
/**
 * @brief  一般功能設定
 * @note   
 * @param  obj: 目標物件
 * @param  st: 設定資料
 * @retval None
 */
void nRF_GeneralSetup( nRF_T* obj, nRF_general_setup_t* st ) {

    //******************************************
    // NRF_REG_CFG
    nRF_MaskWriteRegister(
        obj,
        NRF_REG_CFG,
        (                                                         
            ( st->mask_rx_dr << NRF_REG_CFG_MASK_RX_DR_POS ) |    //
            ( st->mask_tx_ds << NRF_REG_CFG_MASK_TX_DS_POS ) |    //
            ( st->mask_max_rt << NRF_REG_CFG_MASK_MAX_RT_POS ) |  //
            ( st->crc_en << NRF_REG_CFG_EN_CRC_POS ) |            //
            ( st->crc_byte << NRF_REG_CFG_CRCO_POS ) |            //
            ( st->mode << NRF_REG_CFG_PRIM_RX_POS )               //
        ),
        (                                  
            NRF_REG_CFG_MASK_RX_DR_MSK |   //
            NRF_REG_CFG_MASK_TX_DS_MSK |   //
            NRF_REG_CFG_MASK_MAX_RT_MSK |  //
            NRF_REG_CFG_EN_CRC_MSK |       //
            NRF_REG_CFG_CRCO_MSK |         //
            NRF_REG_CFG_PRIM_RX_MSK        //
        ) 
    );

    //******************************************
    // NRF_REG_SETUP_AW
    nRF_MaskWriteRegister(
        obj,
        NRF_REG_SETUP_AW,
        ( 
            ( st->addr_width << NRF_REG_SETUP_AW_POS )  //
        ),
        ( 
            NRF_REG_SETUP_AW_MSK  //
        )
    );

    //******************************************
    // NRF_REG_SETUP_RETR
    nRF_MaskWriteRegister(
        obj,
        NRF_REG_SETUP_RETR,
        ( 
            ( st->ard << NRF_REG_SETUP_RETR_ARD_POS ) |  //
            ( st->arc << NRF_REG_SETUP_RETR_ARC_POS )    //
        ),
        ( 
            NRF_REG_SETUP_RETR_ARD_MSK |  //
            NRF_REG_SETUP_RETR_ARC_MSK    //
        )
    );

    //******************************************
    // NRF_REG_RF_CH
    nRF_MaskWriteRegister(
        obj,
        NRF_REG_RF_CH,
        ( 
            ( st->rf_ch << NRF_REG_RF_CH_POS )  //
        ),
        ( 
            NRF_REG_RF_CH_MSK  //
        )
    );

    //******************************************
    // NRF_REG_RF_SETUP
    nRF_MaskWriteRegister(
        obj,
        NRF_REG_RF_SETUP,
        ( 
            ( st->cont_wave << NRF_REG_RF_SETUP_CONT_WAVE_POS ) | //
            ( ((st->rf_data_rate & 0x2) >> 1) << NRF_REG_RF_SETUP_RF_DR_LOW_POS ) | //
            ( ((st->rf_data_rate & 0x1) >> 0) << NRF_REG_RF_SETUP_RF_DR_HIGH_POS ) | //
            ( st->rf_pwr << NRF_REG_RF_SETUP_RF_PWR_POS )
        ),
        ( 
            NRF_REG_RF_SETUP_CONT_WAVE_MSK |    //
            NRF_REG_RF_SETUP_RF_DR_LOW_MSK |    //
            NRF_REG_RF_SETUP_RF_DR_HIGH_MSK |    //
            NRF_REG_RF_SETUP_RF_PWR_MSK
        )
    );

    //******************************************
    // NRF_REG_FEATURE
    nRF_MaskWriteRegister(
        obj,
        NRF_REG_FEATURE,
        ( 
            ( st->en_dypl << NRF_REG_FEATURE_EN_DPL_POS ) |  //
            (st->en_ack_pay << NRF_REG_FEATURE_EN_ACK_PAY_POS) | //
            (st->en_dyn_ack << NRF_REG_FEATURE_EN_DYN_ACK_POS) //
        ),
        ( 
            NRF_REG_FEATURE_EN_DPL_MSK | //
            NRF_REG_FEATURE_EN_DYN_ACK_MSK | //
            NRF_REG_FEATURE_EN_DYN_ACK_MSK //
        )
    );
    
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

    obj->rxNode[ ch ] = node;

    // 寫入接收位址
    NRF_WRITE_REG_BYTE( obj, NRF_REG_RX_ADDR_P0 + ch, node->addr );

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
        NRF_WRITE_REG_BYTE( obj, ( uint8_t )( NRF_REG_RX_PW_P0 + ch ), node->payloadWide );  // 設定靜態長度
    }

    return true;
}

//***********************************************************************
/**
 * @brief  移除通道
 * @note
 * @param  obj: 目標物件
 * @param  node: 移除的節點結構
 * @retval NoneP
 */
void nRF_RemovalRxNode( nRF_T* obj, nRF_node_t* node, uint8_t ch ) {
    nRF_AndWriteRegister( obj, NRF_REG_EN_RXADDR, ~( 0x1 << ch ) );
}
//***********************************************************************
/**
 * @brief  設定傳輸封包
 * @note
 * @param  obj: 目標物件
 * @param  txPacket: 傳輸封包物件
 * @retval None
 */
void nRF_TxPacketSettingUpdate( nRF_T* obj, nRF_tx_packet_t* txPacket ) {
    bool    enaa          = txPacket->node->autoAckEnabled | txPacket->node->dynamicPayloadLengthEnabled;
    bool    endy          = txPacket->node->dynamicPayloadLengthEnabled;
    uint8_t destAddr[ 5 ] = { txPacket->node->addr,  //
                              txPacket->addrHeader[ 0 ],
                              txPacket->addrHeader[ 1 ],
                              txPacket->addrHeader[ 2 ],
                              txPacket->addrHeader[ 3 ] };
    NRF_WRITE_REG_ARRAY( obj, NRF_REG_TX_ADDR, destAddr, 5 );                                       // TX address
    nRF_MaskWriteRegister( obj, NRF_REG_EN_RXADDR, enaa, NRF_REG_EN_RXADDR_ERX_P0_MSK );            // P0 EN
    nRF_MaskWriteRegister( obj, NRF_REG_EN_AA, enaa, NRF_REG_EN_AA_P0_MSK );                        // P0 ACK
    nRF_MaskWriteRegister( obj, NRF_REG_DYNPD, endy, NRF_REG_DYNPD_DPL_P0_MSK );                    // P0 Dynamic payload
    if ( enaa == true ) NRF_WRITE_REG_ARRAY( obj, NRF_REG_RX_ADDR_P0, destAddr, 5 );                // P0 RX address
    if ( endy == false ) NRF_WRITE_REG_BYTE( obj, NRF_REG_RX_PW_P0, txPacket->node->payloadWide );  // P0 RX payload width
}
//***********************************************************************
/**
 * @brief  讀取有效附載到緩衝區
 * @note
 * @param  obj: 目標物件
 * @param  buf: 目標緩衝區
 * @param  length: 讀取長度，如果為0將視為動態有效附載長度。
 * @retval
 */
static uint8_t ReadPayloadToBuffer( nRF_T* obj, nRF_buf_t* buf, uint8_t length ) {
    uint8_t readLength;
    if ( length == 0 ) { NRF_READ_RX_PAYLOAD_WIDE( obj, &readLength ); }
    else {
        readLength = length;
    }
    NRF_RX_PAYLOAD( obj, buf->data, readLength );
    buf->length = readLength;
    return readLength;
}
//***********************************************************************
/**
 * @brief  傳送封包
 * @note
 * @param  obj: 目標物件
 * @param  txPacket: 傳送封包結構
 * @retval 結果
 */
nRF_statusReg_t nRF_TxPacket( nRF_T* obj, nRF_tx_packet_t* txPacket ) {

    // Dynamic width mode
    if ( txPacket->node->dynamicPayloadLengthEnabled == true )  //
        NRF_TX_PAYLOAD( obj, txPacket->txBuffer->data, txPacket->txBuffer->length );
    else
        NRF_TX_PAYLOAD( obj, txPacket->txBuffer->data, txPacket->node->payloadWide );

    // wait interrupt signal
    while ( obj->interruptFlag == false ) {};

    // 取得中斷狀態
    nRF_statusReg_t status = NRF_NOP( obj );

    if ( status.tx_ds == true ) {
        if ( status.rx_dr == true ) {
            if ( status.rx_p_no == 0x00 )
                ReadPayloadToBuffer( obj, txPacket->node->buf, txPacket->node->payloadWide );
            else
                NRF_FLUSH_RX( obj );
        }
        NRF_WRITE_REG_BYTE( obj, NRF_REG_STATUS, status.dataByte );  // clear status register.
    }
    else if ( status.max_rt == true ) {
    }
}

//***********************************************************************
/**
 * @brief  接收封包
 * @note
 * @param  obj: 目標物件
 * @retval 接收的通道編號
 */
// uint8_t nRF_RxPacket( nRF_T* obj ) {
//     uint8_t interruptFlag = NRF_NOP( obj );
//     uint8_t data_length;
//     nRF_ReadRxPayloadWide( obj, &data_length );

//     if ( obj->rxNode[ 0 ] != NULL &&       //
//          obj->rxNode[ 0 ]->buf != NULL &&  //
//          obj->rxNode[ 0 ]->buf->data != NULL ) {
//         nRF_RxPayload( obj, obj->rxNode[ interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK ]->buf->data, data_length );
//         obj->rxNode[ interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK ]->buf->length = data_length;
//     }
//     else {
//         NRF_FLUSH_RX( obj );
//     }

//     // 清除中斷旗標
//     NRF_WRITE_REG_BYTE( obj, NRF_REG_STATUS, interruptFlag );
//     obj->interruptFlag = false;

//     return interruptFlag & NRF_REG_STATUS_RX_P_NO_MSK;
// }

void nRF_InterruptHookFunciation( nRF_T* obj ) {
    obj->interruptFlag = true;
}
