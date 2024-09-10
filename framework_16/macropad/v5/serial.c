////////////////////////////////////////////////////////////////////////////////
//
// Serial CLI Handling
//
////////////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
//#include "hardware/flash.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/bootrom.h"

#include "i2c.h"
#include "led.h"

#include "macropad.h"
#include "keyboard.h"


void serial_help(void);
void prompt(void);

int parameter = 0;
int keypress = 0;


////////////////////////////////////////////////////////////////////////////////
//
  
void cli_boot_mass(void)
{
  reset_usb_boot(0,0); 
}

void cli_zero_parameter(void)
{
  parameter = 0;
}

// Another digit pressed, update the parameter variable
void cli_digit(void)
{
  unsigned int  n = 0;

#if DB_DIGIT
  printf("\nK:%02X", keypress);
  printf("\nn:%02X", n);
#endif
  if( keypress > '9' )
    {
      n = keypress - 'a' + 10;
    }
  else
    {
      n = keypress - '0';
    }

#if DB_DIGIT
  printf("\nn:%02X", n);
#endif
  
  parameter <<= 4;
  parameter |= n;
}

void cli_scan(void)
{
  
}

void cli_drive_row(void)
{
  drive_row(parameter);
}

void cli_drive_column(void)
{
  for(int c=0; c< NUM_COL; c++)
    {
      drive_column(c, 0);
    }
  
  drive_column(7, 0);
  drive_column(3, 0);
  drive_column(4, 0);
}

void cli_read_adc(void)
{

  int result = adc_read();
  printf("\nADC:%d", result);

}
  

void cli_init_i2c(void)
{
  i2c_init_il31();
}

void cli_send_key(void)
{
  // Wait a bit then send the key
  sleep_ms(3000);
  deliver_key(parameter);  
}



int r, g, b;
int x, y;

void cli_led_test(void)
{


  x = (parameter & 0xFF00) >> 8;
  y = (parameter & 0x00FF) >> 0;
  
  set_led_rgb(parameter, y, r, g, b);
}

void cli_led_test_offset(void)
{
  
  set_page0_rgb(parameter, r, g, b);
  parameter++;
}

void cli_set_rgb(void)
{
  r = (parameter & 0xFF0000) >> 16;
  g = (parameter & 0x00FF00) >>  8;
  b = (parameter & 0x0000FF) >>  0;
}

void cli_set_all_rgb(void)
{
  for(int i=0; i<NUM_LEDS; i++)
    {
      set_led_rgb(i, 0, r, g, b);
    }
}

void cli_inc_param(void)
{
  parameter++;
}

////////////////////////////////////////////////////////////////////////////////

// Serial loop command structure

typedef void (*SERIAL_FPTR)(void);

typedef struct
{
  char key;
  char *desc;
  SERIAL_FPTR fn;
} SERIAL_COMMAND;

SERIAL_COMMAND serial_cmds[] =
  {
   {
    '?',
    "Serial command help",
    serial_help,
   },
   {
    '!',
    "Boot to mass storage",
    cli_boot_mass,
   },
   {
    '0',
    "*Digit",
    cli_digit,
   },
   {
    '1',
    "*Digit",
    cli_digit,
   },
   {
    '2',
    "*Digit",
    cli_digit,
   },
   {
    '3',
    "*Digit",
    cli_digit,
   },
   {
    '4',
    "*Digit",
    cli_digit,
   },
   {
    '5',
    "*Digit",
    cli_digit,
   },
   {
    '6',
    "*Digit",
    cli_digit,
   },
   {
    '7',
    "*Digit",
    cli_digit,
   },
   {
    '8',
    "*Digit",
    cli_digit,
   },
   {
    '9',
    "*Digit",
    cli_digit,
   },
   {
    'a',
    "*Digit",
    cli_digit,
   },
   {
    'b',
    "*Digit",
    cli_digit,
   },
   {
    'c',
    "*Digit",
    cli_digit,
   },
   {
    'd',
    "*Digit",
    cli_digit,
   },
   {
    'e',
    "*Digit",
    cli_digit,
   },
   {
    'f',
    "*Digit",
    cli_digit,
   },
   {
    'z',
    "Zero Parameter",
    cli_zero_parameter,
   },
   {
    'C',
    "Set colour",
    cli_set_rgb,
   },
   {
    'A',
    "Read A/D",
    cli_read_adc,
   },
   {
    'S',
    "Scan",
    cli_scan,
   },
   {
    'I',
    "Init i2c and test LEDs",
    cli_init_i2c,
   },
   {
    'U',
    "Send USB key",
    cli_send_key,
   },
   
   {
    'L',
    "LED test",
    cli_led_test,
   },
   {
    'l',
    "LED test offset",
    cli_led_test_offset,
   },
   {
    '=',
    "All leds to one colour",
    cli_set_all_rgb,
   },
   {
    '+',
    "Inc parameter",
    cli_inc_param,
   },
   
  };

//------------------------------------------------------------------------------


void serial_loop(void)
{
  int key = -1;

  
  int key_queue[3];
  
  if( ((key = getchar_timeout_us(100)) != PICO_ERROR_TIMEOUT))
    {
      for(int i=0; i<sizeof(serial_cmds)/sizeof(SERIAL_COMMAND);i++)
	{
	  if( serial_cmds[i].key == key )
	    {
		  
	      keypress = key;
	      (*serial_cmds[i].fn)();
	      prompt();
	      break;
	    }
	}
    }
}


void serial_help(void)
{
  printf("\n");
  
  for(int i=0; i<sizeof(serial_cmds)/sizeof(SERIAL_COMMAND);i++)
    {
      if( *(serial_cmds[i].desc) != '*' )
	{
	  printf("\n%c:   %s", serial_cmds[i].key, serial_cmds[i].desc);
	}
    }
  
  printf("\nEnter 0-9,A-F for hex parameter");  
  printf("\n");
}


void prompt(void)
{
  printf("\nP:%08X >", parameter);
}

