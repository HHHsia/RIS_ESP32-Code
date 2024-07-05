#ifndef _SPI_Master_For_RIS
#define _SPI_Master_For_RIS

void SPI_Init();
void SPI_Add_Device(uint8_t CS_Pin , spi_device_handle_t* spi , int Clock_Speed);
void SPI_Read(uint8_t reg, uint8_t* val , spi_device_handle_t spi);
void SPI_Write_byte(uint8_t val , spi_device_handle_t spi);
void SPI_Write_16bytes(uint8_t* data , spi_device_handle_t spi);
void SPI_Write_Multi_bytes(uint8_t* data , spi_device_handle_t spi , uint16_t count);     
void SPI_Write_Multi_bytes_V2(uint8_t* data , spi_device_handle_t spi , uint16_t count);                                                          
#endif