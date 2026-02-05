#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define GPS_UART uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 16  // GPS RX接TX
#define UART_RX_PIN 17  // GPS TX接RX

int main() {
    stdio_init_all();
    sleep_ms(1000);
    
    printf("GPS Simple Test\n");
    
    // 初始化UART
    uart_init(GPS_UART, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    printf("UART initialized, waiting for data...\n");
    
    uint32_t last_print = to_ms_since_boot(get_absolute_time());
    
    while (true) {
        if (uart_is_readable(GPS_UART)) {
            char c = uart_getc(GPS_UART);
            // 只打印可打印字符和换行符
            if ((c >= 32 && c <= 126) || c == '\n' || c == '\r') {
                putchar(c);
            }
            // 移除对其他字符的特殊处理，避免输出混乱
        }
        
        // 添加超时机制，避免程序卡死
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (now - last_print > 5000) {  // 每5秒输出一次状态
            printf("\n[Still waiting for GPS data...]\n");
            last_print = now;
        }
        
        sleep_ms(1);
    }
}