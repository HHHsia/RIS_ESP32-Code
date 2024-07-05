#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "SPI_Master_For_RIS.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "rom/ets_sys.h"
#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"

#define Slave1_PIN 17
#define CLOCK_SPEED 2000000

spi_device_handle_t Slave1;
// DATA 23
// CLK  18
// LATCH 17

void UART_Init(){
    esp_console_config_t console_config = {0};
    esp_console_init(&console_config);

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    esp_vfs_dev_uart_use_driver(UART_NUM_0);
    esp_vfs_dev_uart_port_set_rx_line_endings(UART_NUM_0, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(UART_NUM_0, ESP_LINE_ENDINGS_CRLF);    
}
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
        uint8_t i;
        for (i = 0; i < 8; i++)  {
                gpio_set_level(dataPin, val & 1);
                val >>= 1;
                gpio_set_level(clockPin, 1);
                gpio_set_level(clockPin, 0);
        }
}
void app_main(void)
{
    printf("Hello world! \nCurrent SPI Clock Frequency : %d\n", CLOCK_SPEED);
    SPI_Init();
    UART_Init();
    SPI_Add_Device(Slave1_PIN, &Slave1, CLOCK_SPEED);

    uint8_t case_0[50] = { 31, 190, 0, 63, 128, 0, 224, 63, 4, 15, 248, 1, 255, 128, 31, 240, 32, 252, 7, 0, 1, 252, 0, 125, 248, 126, 1, 255, 0, 7, 192, 48, 24, 63, 192, 7, 254, 0, 127, 224, 3, 252, 24, 12, 3, 224, 0, 255, 128, 126 };
    uint8_t case_m10[50] = { 1, 224, 127, 192, 255, 31, 190, 0, 31, 0, 224, 65, 255, 192, 63, 254, 7, 255, 224, 127, 254, 7, 255, 192, 63, 224, 56, 16, 255, 252, 126, 1, 254, 0, 3, 129, 254, 0, 255, 240, 31, 255, 129, 255, 248, 31, 255, 128, 255, 240 };
    uint8_t case_0xAA[2] = {0xAA , 0xAA};
    uint8_t case_0xFF[2] = {0xFF , 0xFF};
    int data;
    while(1){   
        printf("\nEnter Data : ");
        scanf("%d", &data);
        printf("\nData entered : %d\n", data);
        if(data == 0){
            SPI_Write_Multi_bytes(case_0xAA , Slave1 , 2);
            ets_delay_us(10);
        }
        else if (data == -10){
            SPI_Write_Multi_bytes(case_0xFF , Slave1 , 2);
            ets_delay_us(10);
        }
        //vTaskDelay(1000/portTICK_RATE_MS);
    }
}
