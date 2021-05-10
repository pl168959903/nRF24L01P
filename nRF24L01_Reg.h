#ifndef __NRF24L01_REG_H
#define __NRF24L01_REG_H

//-------------------------------------------------
// NRF CMD
#define NRF_CMD_R_REGISTER          0x00
#define NRF_CMD_W_REGISTER          0x20
#define NRF_CMD_R_RX_PAYLOAD        0x61
#define NRF_CMD_W_TX_PAYLOAD        0xA0
#define NRF_CMD_FLUSH_TX            0xE1
#define NRF_CMD_FLUSH_RX            0xE2
#define NRF_CMD_REUSE_TX_PL         0xE3
#define NRF_CMD_R_RX_PL_WID         0x60
#define NRF_CMD_W_ACK_PAYLOAD       0xA8
#define NRF_CMD_W_TX_PAYLOAD_NOACK  0xB0
#define NRF_CMD_NOP                 0xFF
//-------------------------------------------------
// NRF REG
#define NRF_REG_CFG 0x00
#define NRF_REG_CFG_MAKE_RX_RD_POS          (6)
#define NRF_REG_CFG_MAKE_RX_RD_MSK          (0x1<<NRF_REG_CFG_MAKE_RX_RD_POS)
#define NRF_REG_CFG_MAKE_TX_RS_POS          (5)
#define NRF_REG_CFG_MAKE_TX_RS_MSK          (0x1<<NRF_REG_CFG_MAKE_TX_RS_POS)
#define NRF_REG_CFG_MAKE_MAX_RT_POS         (4)
#define NRF_REG_CFG_MAKE_MAX_RT_MSK         (0x1<<NRF_REG_CFG_MAKE_MAX_RT_POS)
#define NRF_REG_CFG_EN_CRC_POS              (3)
#define NRF_REG_CFG_EN_CRC_MSK              (0x1<<NRF_REG_CFG_EN_CRC_POS)
#define NRF_REG_CFG_CRCO_POS                (2)
#define NRF_REG_CFG_CRCO_MSK                (0x1<<NRF_REG_CFG_CRCO_POS)
#define NRF_REG_CFG_PWR_UP_POS              (1)
#define NRF_REG_CFG_PWR_UP_MSK              (0x1<<NRF_REG_CFG_PWR_UP_POS)
#define NRF_REG_CFG_PRIM_RX_POS             (0)
#define NRF_REG_CFG_PRIM_RX_MSK             (0x1<<NRF_REG_CFG_PRIM_RX_POS)

#define NRF_REG_EN_AA                       0x01
#define NRF_REG_EN_AA_P5_POS                (5)
#define NRF_REG_EN_AA_P5_MSK                (0x1<<NRF_REG_EN_AA_P5_POS)
#define NRF_REG_EN_AA_P4_POS                (4)
#define NRF_REG_EN_AA_P4_MSK                (0x1<<NRF_REG_EN_AA_P4_POS)
#define NRF_REG_EN_AA_P3_POS                (3)
#define NRF_REG_EN_AA_P3_MSK                (0x1<<NRF_REG_EN_AA_P3_POS)
#define NRF_REG_EN_AA_P2_POS                (2)
#define NRF_REG_EN_AA_P2_MSK                (0x1<<NRF_REG_EN_AA_P2_POS)
#define NRF_REG_EN_AA_P1_POS                (1)
#define NRF_REG_EN_AA_P1_MSK                (0x1<<NRF_REG_EN_AA_P1_POS)
#define NRF_REG_EN_AA_P0_POS                (0)
#define NRF_REG_EN_AA_P0_MSK                (0x1<<NRF_REG_EN_AA_P0_POS)

#define NRF_REG_EN_RXADDR                   0x02
#define NRF_REG_EN_RXADDR_ERX_P5_POS        (5)
#define NRF_REG_EN_RXADDR_ERX_P5_MSK        (0x1<<NRF_REG_EN_RXADDR_ERX_P5_POS)
#define NRF_REG_EN_RXADDR_ERX_P4_POS        (4)
#define NRF_REG_EN_RXADDR_ERX_P4_MSK        (0x1<<NRF_REG_EN_RXADDR_ERX_P4_POS)
#define NRF_REG_EN_RXADDR_ERX_P3_POS        (3)
#define NRF_REG_EN_RXADDR_ERX_P3_MSK        (0x1<<NRF_REG_EN_RXADDR_ERX_P3_POS)
#define NRF_REG_EN_RXADDR_ERX_P2_POS        (2)
#define NRF_REG_EN_RXADDR_ERX_P2_MSK        (0x1<<NRF_REG_EN_RXADDR_ERX_P2_POS)
#define NRF_REG_EN_RXADDR_ERX_P1_POS        (1)
#define NRF_REG_EN_RXADDR_ERX_P1_MSK        (0x1<<NRF_REG_EN_RXADDR_ERX_P1_POS)
#define NRF_REG_EN_RXADDR_ERX_P0_POS        (0)
#define NRF_REG_EN_RXADDR_ERX_P0_MSK        (0x1<<NRF_REG_EN_RXADDR_ERX_P0_POS)

#define NRF_REG_SETUP_AW                    0x03
#define NRF_REG_SETUP_AW_POS                (0)
#define NRF_REG_SETUP_AW_MSK                (0x3<<NRF_REG_SETUP_AW_POS)

#define NRF_REG_SETUP_RETR                  0x04
#define NRF_REG_SETUP_RETR_ARD_POS          (4)
#define NRF_REG_SETUP_RETR_ARD_MSK          (0xF<<NRF_REG_SETUP_RETR_ARD_POS)
#define NRF_REG_SETUP_RETR_ARC_POS          (0)
#define NRF_REG_SETUP_RETR_ARC_MSK          (0xF<<NRF_REG_SETUP_RETR_ARC_POS)

#define NRF_REG_RF_CH                       0x05
#define NRF_REG_RF_CH_POS                   (0)
#define NRF_REG_RF_CH_MSK                   (0x7F<<NRF_REG_RF_CH_POS)

#define NRF_REG_RF_SETUP                    0x06
#define NRF_REG_RF_SETUP_PLL_LOCK_POS       (4)
#define NRF_REG_RF_SETUP_PLL_LOCK_MSK       (0x1<<NRF_REG_RF_SETUP_PLL_LOCK_POS)
#define NRF_REG_RF_SETUP_RF_DR_POS          (3)
#define NRF_REG_RF_SETUP_RF_DR_MSK          (0x1<<NRF_REG_RF_SETUP_RF_DR_POS)
#define NRF_REG_RF_SETUP_RF_PWR_POS         (1)
#define NRF_REG_RF_SETUP_RF_PWR_MSK         (0x3<<NRF_REG_RF_SETUP_RF_PWR_POS)
#define NRF_REG_RF_SETUP_LNA_HCURR_POS      (0)
#define NRF_REG_RF_SETUP_LNA_HCURR_MSK      (0x1<<NRF_REG_RF_SETUP_LNA_HCURR_POS)

#define NRF_REG_STATUS                      0x07
#define NRF_REG_STATUS_RX_DR_POS            (6)
#define NRF_REG_STATUS_RX_DR_MSK            (0x1<<NRF_REG_STATUS_RX_DR_POS)
#define NRF_REG_STATUS_TX_DS_POS            (5)
#define NRF_REG_STATUS_TX_DS_MSK            (0x1<<NRF_REG_STATUS_TX_DS_POS)
#define NRF_REG_STATUS_MAX_RT_POS           (4)
#define NRF_REG_STATUS_MAX_RT_MSK           (0x1<<NRF_REG_STATUS_MAX_RT_POS)
#define NRF_REG_STATUS_RX_P_NO_POS          (1)
#define NRF_REG_STATUS_RX_P_NO_MSK          (0x7<<NRF_REG_STATUS_RX_P_NO_POS)
#define NRF_REG_STATUS_TX_FULL_POS          (0)
#define NRF_REG_STATUS_TX_FULL_MSK          (0x1<<NRF_REG_STATUS_TX_FULL_POS)

#define NRF_REG_OBSERVE_TX                  0x08
#define NRF_REG_OBSERVE_TX_PLOS_CNT_POS     (4)
#define NRF_REG_OBSERVE_TX_PLOS_CNT_MSK     (0xF<<NRF_REG_OBSERVE_TX_PLOS_CNT_POS)
#define NRF_REG_OBSERVE_TX_ARC_CNT_POS      (0)
#define NRF_REG_OBSERVE_TX_ARC_CNT_MSK      (0xF<<NRF_REG_OBSERVE_TX_ARC_CNT_POS)

#define NRF_REG_CD                          0x09
#define NRF_REG_CD_POS                      (0)
#define NRF_REG_CD_MSK                      (0x1<<NRF_REG_CD_POS)

#define NRF_REG_RX_ADDR_P0                  0x0A
#define NRF_REG_RX_ADDR_P1                  0x0B
#define NRF_REG_RX_ADDR_P2                  0x0C
#define NRF_REG_RX_ADDR_P3                  0x0D
#define NRF_REG_RX_ADDR_P4                  0x0E
#define NRF_REG_RX_ADDR_P5                  0x0F

#define NRF_REG_TX_ADDR                     0x10

#define NRF_REG_RX_PW_P0                    0x11
#define NRF_REG_RX_PW_P0_POS                (0)
#define NRF_REG_RX_PW_P0_MSK                (0x3F<<NRF_REG_RX_PW_P0_POS)

#define NRF_REG_RX_PW_P1                    0x12
#define NRF_REG_RX_PW_P1_POS                (0)
#define NRF_REG_RX_PW_P1_MSK                (0x3F<<NRF_REG_RX_PW_P1_POS)

#define NRF_REG_RX_PW_P2                    0x13
#define NRF_REG_RX_PW_P2_POS                (0)
#define NRF_REG_RX_PW_P2_MSK                (0x3F<<NRF_REG_RX_PW_P2_POS)

#define NRF_REG_RX_PW_P3                    0x14
#define NRF_REG_RX_PW_P3_POS                (0)
#define NRF_REG_RX_PW_P3_MSK                (0x3F<<NRF_REG_RX_PW_P3_POS)

#define NRF_REG_RX_PW_P4                    0x15
#define NRF_REG_RX_PW_P4_POS                (0)
#define NRF_REG_RX_PW_P4_MSK                (0x3F<<NRF_REG_RX_PW_P4_POS)

#define NRF_REG_RX_PW_P5                    0x16
#define NRF_REG_RX_PW_P5_POS                (0)
#define NRF_REG_RX_PW_P5_MSK                (0x3F<<NRF_REG_RX_PW_P5_POS)

#define NRF_REG_FIFO_STATUS                 0x17
#define NRF_REG_FIFO_STATUS_TX_REUSE_POS    (6)
#define NRF_REG_FIFO_STATUS_TX_REUSE_MSK    (0x1<<NRF_REG_FIFO_STATUS_TX_REUSE_POS)
#define NRF_REG_FIFO_STATUS_TX_FULL_POS     (5)
#define NRF_REG_FIFO_STATUS_TX_FULL_MSK     (0x1<<NRF_REG_FIFO_STATUS_TX_FULL_POS)
#define NRF_REG_FIFO_STATUS_TX_EMPTY_POS    (4)
#define NRF_REG_FIFO_STATUS_TX_EMPTY_MSK    (0x1<<NRF_REG_FIFO_STATUS_TX_EMPTY_POS)
#define NRF_REG_FIFO_STATUS_RX_FULL_POS     (1)
#define NRF_REG_FIFO_STATUS_RX_FULL_MSK     (0x1<<NRF_REG_FIFO_STATUS_RX_FULL_POS)
#define NRF_REG_FIFO_STATUS_RX_EMPTY_POS    (0)
#define NRF_REG_FIFO_STATUS_RX_EMPTY_MSK    (0x1<<NRF_REG_FIFO_STATUS_RX_EMPTY_POS)

#define NRF_REG_DYNPD                       0x1C
#define NRF_REG_DYNPD_DPL_P5_POS            (5)               
#define NRF_REG_DYNPD_DPL_P5_MSK            (0x1<<NRF_REG_DYNPD_DPL_P5_POS)              
#define NRF_REG_DYNPD_DPL_P4_POS            (4)               
#define NRF_REG_DYNPD_DPL_P4_MSK            (0x1<<NRF_REG_DYNPD_DPL_P4_POS)              
#define NRF_REG_DYNPD_DPL_P3_POS            (3)               
#define NRF_REG_DYNPD_DPL_P3_MSK            (0x1<<NRF_REG_DYNPD_DPL_P3_POS)              
#define NRF_REG_DYNPD_DPL_P2_POS            (2)               
#define NRF_REG_DYNPD_DPL_P2_MSK            (0x1<<NRF_REG_DYNPD_DPL_P2_POS)              
#define NRF_REG_DYNPD_DPL_P1_POS            (1)               
#define NRF_REG_DYNPD_DPL_P1_MSK            (0x1<<NRF_REG_DYNPD_DPL_P1_POS)              
#define NRF_REG_DYNPD_DPL_P0_POS            (0)               
#define NRF_REG_DYNPD_DPL_P0_MSK            (0x1<<NRF_REG_DYNPD_DPL_P0_POS) 

#define NRF_REG_FEATURE                     0x1D
#define NRF_REG_FEATURE_EN_DPL_POS          (2)
#define NRF_REG_FEATURE_EN_DPL_MSK          (0x1<<NRF_REG_FEATURE_EN_DPL_POS)
#define NRF_REG_FEATURE_EN_ACK_POS          (1)
#define NRF_REG_FEATURE_EN_ACK_MSK          (0x1<<NRF_REG_FEATURE_EN_ACK_POS)
#define NRF_REG_FEATURE_EN_DYN_ACK_POS      (0)
#define NRF_REG_FEATURE_EN_DYN_ACK_MSK      (0x1<<NRF_REG_FEATURE_EN_DYN_ACK_POS)

//-------------------------------------------------


#endif /* __NRF24L01_REG_H */
