
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
void i2c_write_byte(uint8_t *reg, uint8_t *val)
{
  uint8_t buf[2];

  buf[0] = *reg;
  buf[1] = *val;
  
  //i2c_write_blocking(I2C_BUS, IL31_ADDR, reg, 1, true);
  i2c_write_blocking(I2C_BUS, IL31_ADDR, buf, 2, false);
}


uint8_t i2c_read_byte(uint8_t *reg)
{
  uint8_t data;
  
  i2c_write_blocking(I2C_BUS, IL31_ADDR, reg, 1, true);
  i2c_read_blocking(I2C_BUS, IL31_ADDR, &data, 1, false);

  return(data);
}

void i2c_unlock_il31(void)
{
  uint8_t reg = 0xFE;
  uint8_t data = 0xC5;
  
  i2c_write_byte(&reg, &data);
}

uint8_t il_id = 0;

void i2c_init_il31(void)
{
  uint8_t data;
  uint8_t reg;

  i2c_init(I2C_BUS, 10 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  
  // Read ID register
  reg = 0xFC;
  il_id = i2c_read_byte(&reg);

  printf("\n%s:IL31 ID:%02X", __FUNCTION__, il_id);

  // Write config byte
  i2c_unlock_il31();

  reg = 0xFE;
  data = i2c_read_byte(&reg);
  printf("\nUnlock register:%02X", data);


  reg = 0xFD;
  data = 0x02;
  i2c_write_byte(&reg, &data);
  data = i2c_read_byte(&reg);
  printf("\nFD:%02X", data);

  reg = 0x00;
  data = i2c_read_byte(&reg);
  printf("\nConfig register:%02X", data);

  reg = 0x00;
  data = 0x71;
  i2c_write_byte(&reg, &data);

  // Global current
  reg = 0x01;
  data = 70;
  i2c_write_byte(&reg, &data);

  reg = 0x02;
  data = 0x33;
  i2c_write_byte(&reg, &data);

  sleep_ms(10);
  
  // Quick turn on of all LEDs
  i2c_unlock_il31();
  reg = 0xFD;
  data = 0x00;
  i2c_write_byte(&reg, &data);

  data = 0;
  for(int i = 1; i<= 0xC6; i++)
    {
      reg = i;
      data +=13;

      i2c_write_byte(&reg, &data);
    }

  i2c_unlock_il31();
  reg = 0xFD;
  data = 0x01;
  i2c_write_byte(&reg, &data);

  for(int i = 1; i<= 0xC6; i++)
    {
      reg = i;
      data += 17;

      i2c_write_byte(&reg, &data);
    }


  // Read config byte
  i2c_unlock_il31();
  reg = 0xFD;
  data = 0x02;
  i2c_write_byte(&reg, &data);
  

  reg = 0x00;

  data = i2c_read_byte(&reg);

  printf("\nConfig register:%02X", data);

  
  reg = 0xFE;
  data = i2c_read_byte(&reg);
  printf("\nUnlock register:%02X", data);

  i2c_unlock_il31();

  reg = 0xFD;
  data = 0x00;
  i2c_write_byte(&reg, &data);
  reg = 0x01;
  data = i2c_read_byte(&reg);
  printf("\nPG0:0x00:%02X", data);
}

