
#include <stdlib.h>

#include "nRF24L01_Obj.h"
#include "nRF24L01_Reg.h"
#include "nRF24L01_Base_API.h"
#include "nRF24L01_Drv.h"
#include "nRF24L01_Pair.h"

void nRF_PairServerInit(nRF_T* obj, nRF_pair_t *pair){
    nRF_ReadRegByte( obj, NRF_REG_EN_RXADDR, &pair->enrx);
    nRF_ReadRegByte( obj, NRF_REG_EN_AA, &pair->enaa);
    nRF_ReadRegByte( obj, NRF_REG_DYNPD, &pair->dynpd);

    nRF_WriteRegByte( obj, NRF_REG_EN_RXADDR, 0x01); 
    nRF_WriteRegByte( obj, NRF_REG_EN_AA, 0x01);
    nRF_WriteRegByte( obj, NRF_REG_DYNPD, 0x01);

    nRF_WriteRegArray( obj, NRF_REG_RX_ADDR_P0, pair->PairAddress, 5);
}
