/*
 * @file logMessage.c
 * @brief print method to uartStr
 * @date 04/10/26
 * @author Isaac Cormier
 */

#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "manual.h"
/*
 * @brief prints whatever we want to uartStr (designed to bypass the fact that uartStr can only take 1 arg, which makes it annoying
 * to print values to uart without having another buffer and sprintf) definetly did all of this by myself with no outside help
 * @author Isaac Cormier
 * @param str buffer size, then message of what you want to print, formatted like normal!
 * @date 04/14/26
 */
void log_message(size_t buf_Size, const char *fmt, ...)
{

    char buffer[bufSize];

    va_list args;
    va_start(args, fmt);

    vsnprintf(buffer, buf_Size, fmt, args);

    va_end(args);

    // Output to LCD
    // lcd_clear();
    // lcd_printf("%s", buffer);

    // Output to UART
    uart_sendStr(buffer);
}
