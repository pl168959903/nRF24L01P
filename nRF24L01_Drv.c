#include <stdbool.h>

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
    nRF_WriteRegByte( obj, NRF_REG_EN_AA, 0x3F);
    nRF_WriteRegByte( obj, NRF_REG_EN_RXADDR, 0x03);
    nRF_WriteRegByte( obj, NRF_REG_SETUP_AW, 0x03);
    nRF_WriteRegByte( obj, NRF_REG_SETUP_RETR, 0x03);
    nRF_WriteRegByte( obj, NRF_REG_RF_CH, 0x02);
    nRF_WriteRegByte( obj, NRF_REG_RF_SETUP, 0x0F);
    nRF_WriteRegByte( obj, NRF_REG_OBSERVE_TX, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_CD, 0x00);
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P0, (uint8_t[]){0xE7, 0xE7, 0xE7, 0xE7, 0xE7}, 5);
    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P1, (uint8_t[]){0xC2, 0xC2, 0xC2, 0xC2, 0xC2}, 5);
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P2, 0xC3);
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P3, 0xC4);
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P4, 0xC5);
    nRF_WriteRegByte( obj, NRF_REG_RX_ADDR_P5, 0xC6);
    nRF_WriteRegArray( obj, NRF_REG_TX_ADDR, (uint8_t[]){0xE7, 0xE7, 0xE7, 0xE7, 0xE7}, 5);
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P0, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P1, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P2, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P3, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P4, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_RX_PW_P5, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_FIFO_STATUS, 0x11);
    nRF_WriteRegByte( obj, NRF_REG_DYNPD, 0x00);
    nRF_WriteRegByte( obj, NRF_REG_FEATURE, 0x00);
}
