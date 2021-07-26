#ifndef __NRF24L01_DRV_H
#define __NRF24L01_DRV_H

#include "nRF24L01_Base.h"
#include "nRF24L01_Config.h"

#define NRF_CRC_1_BYTES           0
#define NRF_CRC_2_BYTES           1
#define NRF_MODE_PTX              0
#define NRF_MODE_PRX              1
#define NRF_ADDR_WIDTH_3_BYTES    1
#define NRF_ADDR_WIDTH_4_BYTES    2
#define NRF_ADDR_WIDTH_5_BYTES    3
#define NRF_RF_DATA_RATE_1_MBPS   0
#define NRF_RF_DATA_RATE_2_MBPS   1
#define NRF_RF_DATA_RATE_250_KBPS 2
#define NRF_RF_PWR_N_18_DBM       0
#define NRF_RF_PWR_N_12_DBM       1
#define NRF_RF_PWR_N_6_DBM        2
#define NRF_RF_PWR_N_0_DBM        0
#define NRF_RF_CH_MHZ( ch )       ( ch - 2400 )        // ch : 2400MHz ~ 2525MHz
#define NRF_ARD_US( t )           ( ( t / 250 ) - 1 )  // t : 250us ~ 4000us

typedef struct {
    bool    mask_rx_dr : 1;
    bool    mask_tx_ds : 1;
    bool    mask_max_rt : 1;
    bool    crc_en : 1;
    uint8_t crc_byte : 1;
    uint8_t mode : 1;

    uint8_t addr_width : 2;

    uint8_t ard : 4;
    uint8_t arc : 4;

    uint8_t rf_ch : 6;

    bool    cont_wave : 1;
    uint8_t rf_data_rate : 2;
    uint8_t rf_pwr : 2;

    bool en_dypl : 1;
    bool en_ack_pay : 1;
    bool en_dyn_ack : 1;
} nRF_general_setup_t;

typedef enum {
    
}nRF_retval_e;



/**
 * @brief  設定 RX 位址
 * @note
 * @param ch: 通道
 * @param addr: 位址
 * @retval nRF 狀態暫存器
 */
#define NRF_SET_RX_ADDRESS( ch, addr ) NRF_WRITE_REG_ARRAY( obj, NRF_REG_RX_ADDR_P0 + ch, addr, 5 );

#define NRF_POWER_ON( obj )   nRF_OrWriteRegister( obj, NRF_REG_CFG, NRF_REG_CFG_PWR_UP_MSK )
#define NRF_POWER_DOWN( obj ) nRF_AndWriteRegister( obj, NRF_REG_CFG, NRF_REG_CFG_PWR_UP_MSK )
#define NRF_RX_MODE( obj )    nRF_OrWriteRegister( obj, NRF_REG_CFG, NRF_REG_CFG_PRIM_RX_MSK )
#define NRF_TX_MODE( obj )    nRF_AndWriteRegister( obj, NRF_REG_CFG, ~NRF_REG_CFG_PRIM_RX_MSK )

void            nRF_OrWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value );
void            nRF_AndWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value );
void            nRF_MaskWriteRegister( nRF_T* obj, uint8_t reg, uint8_t value, uint8_t mask );
void            nRF_ClearInterruptFlag( nRF_T* obj );
void            nRF_Init( nRF_T* obj );
void            nRF_SetAddressHeader_P0( nRF_T* obj, uint8_t RxAddressHeader[] );
void            nRF_SetAddressHeader_P1_6( nRF_T* obj, uint8_t RxAddressHeader[] );
bool            nRF_AddRxNode( nRF_T* obj, nRF_node_t* node, uint8_t ch );
void            nRF_RemovalRxNode( nRF_T* obj, nRF_node_t* node, uint8_t ch );
nRF_statusReg_t nRF_TxPacket( nRF_T* obj, nRF_tx_packet_t* txPacket );
uint8_t         nRF_RxPacket( nRF_T* obj );

void nRF_InterruptHookFunciation( nRF_T* obj );

#endif /* __NRF24L01_DRV_H */
