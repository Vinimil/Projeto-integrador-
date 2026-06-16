#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdio.h>

#define LCD_I2C i2c0

#define LCD_SDA 8
#define LCD_SCL 9

#define LCD_ADDR 0x27

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04

static void lcd_write4(uint8_t data)
{
    uint8_t buf;

    buf = data | LCD_BACKLIGHT;
    i2c_write_blocking(LCD_I2C, LCD_ADDR, &buf, 1, false);

    sleep_us(1);

    buf |= LCD_ENABLE;
    i2c_write_blocking(LCD_I2C, LCD_ADDR, &buf, 1, false);

    sleep_us(1);

    buf &= ~LCD_ENABLE;
    i2c_write_blocking(LCD_I2C, LCD_ADDR, &buf, 1, false);

    sleep_us(50);
}

static void lcd_send(uint8_t value, uint8_t mode)
{
    uint8_t high =
        (value & 0xF0) | mode;

    uint8_t low =
        ((value << 4) & 0xF0) | mode;

    lcd_write4(high);
    lcd_write4(low);
}

void lcd_cmd(uint8_t cmd)
{
    lcd_send(cmd, 0);
}

void lcd_data(uint8_t data)
{
    lcd_send(data, 1);
}

void lcd_clear(void)
{
    lcd_cmd(0x01);
    sleep_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row)
{
    uint8_t addr =
        row ? 0x40 : 0x00;

    lcd_cmd(0x80 | (addr + col));
}

void lcd_print(const char *text)
{
    while(*text)
    {
        lcd_data(*text++);
    }
}

void lcd_init_display(void)
{
    i2c_init(LCD_I2C, 100000);

    gpio_set_function(LCD_SDA, GPIO_FUNC_I2C);
    gpio_set_function(LCD_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(LCD_SDA);
    gpio_pull_up(LCD_SCL);

    sleep_ms(50);

    lcd_write4(0x30);
    sleep_ms(5);

    lcd_write4(0x30);
    sleep_ms(5);

    lcd_write4(0x30);
    sleep_ms(5);

    lcd_write4(0x20);
    sleep_ms(5);

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);

    sleep_ms(5);
}

void lcd_tela_pronto(float volume)
{
    char linha1[17];
    char linha2[17];

    snprintf(linha1, sizeof(linha1),
             "Volume %.0fml", volume);

    snprintf(linha2, sizeof(linha2),
             "200 250 300 350");

    lcd_clear();

    lcd_set_cursor(0, 0);
    lcd_print(linha1);

    lcd_set_cursor(0, 1);
    lcd_print(linha2);
}

void lcd_tela_dosando(float dose)
{
    char linha1[17];
    char linha2[17];

    snprintf(linha1, sizeof(linha1),
             "DOSANDO");

    snprintf(linha2, sizeof(linha2),
             "%.0f ml", dose);

    lcd_clear();

    lcd_set_cursor(0, 0);
    lcd_print(linha1);

    lcd_set_cursor(0, 1);
    lcd_print(linha2);
}

void lcd_tela_final(float dose)
{
    char linha1[17];
    char linha2[17];

    snprintf(linha1, sizeof(linha1),
             "CONCLUIDO");

    snprintf(linha2, sizeof(linha2),
             "%.0f ml", dose);

    lcd_clear();

    lcd_set_cursor(0, 0);
    lcd_print(linha1);

    lcd_set_cursor(0, 1);
    lcd_print(linha2);
}
