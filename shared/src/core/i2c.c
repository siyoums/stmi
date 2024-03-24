#include "core/i2c.h"
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

// lcd defines
#define LCD_I2C_ADDRESS 0x27 // Adjust this to your PCF8574's actual address
#define LCD_BACKLIGHT 0x08   // Assuming P3 controls the backlight
#define LCD_ENABLE 0x04      // Assuming P2 is connected to the LCD's E pin
#define LCD_RS 0x01          // Assuming P0 is connected to the LCD's RS pin

void i2c_setup(void) {
  rcc_periph_clock_enable(RCC_I2C2);
  rcc_periph_reset_pulse(RST_I2C2);

  // i2c config
  i2c_peripheral_disable(I2C2);
  i2c_set_clock_frequency(I2C2, 36);

  // scl freq
  i2c_set_standard_mode(I2C2); // 100khz
  i2c_set_ccr(I2C2, 180);

  i2c_set_trise(I2C2, 37);

  i2c_peripheral_enable(I2C2);
}

void i2c_send_byte(uint8_t addr, uint8_t data) {
  i2c_transfer7(I2C2, addr, &data, 1, NULL, 0);
}

// Write a nibble (4 bits) to the LCD
void lcd_write_nibble(uint8_t data, bool is_command) {
  uint8_t control_bits = LCD_BACKLIGHT | (is_command ? 0 : LCD_RS);
  uint8_t high_nibble = control_bits | (data & 0xF0) | LCD_ENABLE;
  uint8_t low_nibble = control_bits | (data & 0xF0);

  i2c_send_byte(LCD_I2C_ADDRESS, high_nibble);
  i2c_send_byte(LCD_I2C_ADDRESS,
                low_nibble); // Toggle enable pin to process the nibble
}

// Write a full byte to the LCD
void lcd_write_byte(uint8_t data, bool is_command) {
  lcd_write_nibble(data & 0xF0, is_command); // Send high nibble
  lcd_write_nibble(data << 4, is_command);   // Send low nibble
}

void lcd_send_char(char c) {
  lcd_write_byte(c, false); // false indicating it's data, not a command
}
