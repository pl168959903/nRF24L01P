#ifndef __NRF24L01_DRV_H
#define __NRF24L01_DRV_H

#include "nRF24L01_Reg.h"

#define NRF_POWER_ON(obj) nRF_OrWriteRegister(obj, NRF_REG_CFG, NRF_REG_CFG_PWR_UP_MSK)
#define NRF_POWER_DOWN(obj) nRF_AndWriteRegister(obj, NRF_REG_CFG, NRF_REG_CFG_PWR_UP_MSK)
#define NRF_RX_MODE(obj) nRF_OrWriteRegister(obj, NRF_REG_CFG, NRF_REG_CFG_PRIM_RX_MSK)
#define NRF_TX_MODE(obj) nRF_AndWriteRegister(obj, NRF_REG_CFG, ~NRF_REG_CFG_PRIM_RX_MSK)





#endif /* __NRF24L01_DRV_H */
