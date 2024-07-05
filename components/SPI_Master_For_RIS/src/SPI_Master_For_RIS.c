#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "hal/spi_hal.h"

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18

void SPI_Init()
{
    spi_bus_config_t bus_cfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092
    };

    spi_bus_initialize(HSPI_HOST, &bus_cfg, 1);

}
void SPI_Add_Device(uint8_t CS_Pin , spi_device_handle_t* spi , int Clock_Speed){
    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = Clock_Speed,
        .mode = 0,
        .spics_io_num = CS_Pin,
        .queue_size = 1,
        .flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_3WIRE
    };

    spi_bus_add_device(HSPI_HOST, &dev_cfg, spi);

}
void SPI_Read(uint8_t reg, uint8_t* val , spi_device_handle_t spi)
{
    uint8_t tx_data[1] = {reg | 0x80};
    uint8_t rx_data[1] = {0};

    spi_transaction_t tx = {
        .length = 8,
        .tx_buffer = tx_data,
        .rx_buffer = rx_data
    };
    spi_device_transmit(spi, &tx);

    *val = rx_data[0];
}
void SPI_Write_byte(uint8_t val , spi_device_handle_t spi)
{
    uint8_t tx_data[1] = {val};

    spi_transaction_t tx = {
        .length = 8,
        .tx_buffer = tx_data,
        .rx_buffer = NULL
    };
    spi_device_transmit(spi, &tx);
}
void SPI_Write_16bytes(uint8_t* data , spi_device_handle_t spi)
{
    uint8_t tx_data[16];
    memcpy(tx_data, data, 16);

    spi_transaction_t tx = {
        .length = 16 * 8,
        .tx_buffer = tx_data,
        .rx_buffer = NULL
    };
    spi_device_transmit(spi, &tx);
}
void SPI_Write_Multi_bytes(uint8_t* data , spi_device_handle_t spi , uint16_t count)
{
    spi_transaction_t tx = {
        .length = count * 8,
        .tx_buffer = data,
        .rx_buffer = NULL
    };
    spi_device_transmit(spi, &tx);
}
void SPI_Write_Multi_bytes_V2(uint8_t* data , spi_device_handle_t spi , uint16_t count)
{
    spi_transaction_t tx = {
        .length = count * 8,
        .tx_buffer = data,
        .rx_buffer = NULL
    };
    //spi_device_transmit(spi, &tx);
    spi_device_queue_trans_V2(spi , &tx , portMAX_DELAY);
}
