#include <stdint.h>
#include <stdbool.h>
#include "Project.h"

extern void SpiReadAndWriteByPdma( SPI_T* spi, uint8_t tdata[], uint8_t rdata[], uint32_t dataSize );

void nRF0_ReadAndWrite( uint8_t readBuf[], uint8_t writeBuf[], size_t size){
    SpiReadAndWriteByPdma(_NRF_SPI, writeBuf, readBuf, size);
}

void nRF0_CE(bool cePin){
    _NRF_CE_PIN = cePin;
}


