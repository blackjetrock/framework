
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include <ctype.h>

#include "i2c.h"

////////////////////////////////////////////////////////////////////////////////

// I2C address
// ADDr1 and 2 connected to GND

#define IL31_ADDR 0x20
#define I2C_BUS   i2c1
#define SDA_PIN   26
#define SCL_PIN   27


/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val)
{
  i2c_write_blocking(I2C_BUS, IL31_ADDR, &val, 1, false);
}


uint8_t i2c_read_byte(uint8_t reg)
{
  uint8_t data;
  
  i2c_write_blocking(I2C_BUS, IL31_ADDR, &reg, 1, true);
  i2c_read_blocking(I2C_BUS, IL31_ADDR, &data, 1, false);

  return(data);
}

uint8_t il_id = 0;

void i2c_init_il31(void)
{
  i2c_init(I2C_BUS, 10 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  
  // Read ID register
  il_id = i2c_read_byte(0xFC);

  printf("\n%s:%02X", __FUNCTION__, il_id);
  
}

