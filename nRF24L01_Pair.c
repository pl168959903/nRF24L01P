
#include <stdlib.h>

#include "nRF24L01_Base_API.h"
#include "nRF24L01_Drv.h"
#include "nRF24L01_Obj.h"
#include "nRF24L01_Pair.h"
#include "nRF24L01_Reg.h"

void nRF_PairServerInit( nRF_T* obj, nRF_pair_t* pair ) {
    obj->interface->SetCE( false );

    // P0通道啟用、自動Ack、動態負載長度
    nRF_WriteRegByte( obj, NRF_REG_EN_RXADDR, 0x01 );
    nRF_WriteRegByte( obj, NRF_REG_EN_AA, 0x01 );
    nRF_WriteRegByte( obj, NRF_REG_DYNPD, 0x01 );

    nRF_OrWriteRegister( obj, NRF_REG_CFG, 0x01 );  // RX mode.

    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P0, pair->PairAddress, 5 );  // 設定公用頻道

    // 清除RX、TX緩衝區
    nRF_FlushRx( obj); nRF_FlushTx( obj);

    // 清除中斷旗標
    uint8_t status;
    status = nRF_Nop( obj);
    nRF_WriteRegByte( obj, NRF_REG_STATUS, status);

    nRF_AckPayload( obj, (uint8_t[]){pair->token}, 1, 0); 
}

void nRF_PairServerListening( nRF_T* obj, nRF_pair_t pair){
    while(obj->interruptFlag == false){};
    uint8_t status = nRF_Nop( obj);
    
}

