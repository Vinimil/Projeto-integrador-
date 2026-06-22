#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

void lcd_init_display(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(const char *text);

#endif
