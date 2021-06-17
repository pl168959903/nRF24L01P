
#include <stdlib.h>

#include "nRF24L01_Base_API.h"
#include "nRF24L01_Drv.h"
#include "nRF24L01_Obj.h"
#include "nRF24L01_Pair.h"
#include "nRF24L01_Reg.h"

static const uint8_t pairPublicAddress[5] = {
    #include "nRF24L01_pairPublicAddress.txt"
};

void nRF_PairServerInit( nRF_T* obj ) {
    obj->interface->SetCE( false );
    NRF_POWER_DOWN( obj);

    // 建立節點
    uint8_t *pairBufferArray = malloc(32); 
    nRF_buf_t *pairBuffer = malloc(sizeof(nRF_buf_t));
    nRF_node_t *pairNode = malloc(sizeof(nRF_node_t));

    // 參數設定
    *pairBuffer = (nRF_buf_t){pairBufferArray, 0};
    *pairNode = (nRF_node_t){pairPublicAddress[0], NULL, true, true, pairBuffer};

    nRF_AddRxNode( obj, pairNode, 0); // 添加配對節點

    nRF_WriteRegArray(obj, NRF_REG_RX_ADDR_P0, (uint8_t*)pairPublicAddress, 5); // 設定公用地址

    // 清除RX、TX緩衝區
    nRF_FlushRx( obj); nRF_FlushTx( obj);

    // 清除中斷旗標
    uint8_t status;
    status = nRF_Nop( obj);
    nRF_WriteRegByte( obj, NRF_REG_STATUS, status); 
}

void nRF_PairServerListening( nRF_T* obj, nRF_node_t* node){
    uint8_t token = rand() % 256;
    nRF_AckPayload( obj, (uint8_t[]){token}, 1, 0); // Ack 負載傳輸 token
    obj->interface->SetCE( true );  // 啟動
    NRF_POWER_ON(obj);

    while(obj->interruptFlag == false){}; 

    uint8_t status = nRF_Nop( obj); // 取得狀態
    
    if(status & NRF_REG_STATUS_RX_DR_MSK){
        uint8_t dataSize;
        nRF_ReadRxPayloadWide( obj, &dataSize); // 取得長度
    }
    if(status & NRF_REG_STATUS_TX_DS_MSK){

    }
    if(status & NRF_REG_STATUS_MAX_RT_MSK){

    }
}
