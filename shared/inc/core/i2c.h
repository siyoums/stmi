#ifndef INC_I2C_H
#define INC_I2C_H

#include "common-defines.h"

void i2c_setup(void);
void i2c_send_byte(uint8_t addr, uint8_t data);
void lcd_write_byte(uint8_t data, bool is_command);
void lcd_send_char(char c);
void lcd_write_nibble(uint8_t data, bool is_command);
#endif // !INC_I2C_H