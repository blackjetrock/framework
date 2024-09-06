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

#include "macropad.h"

void serial_help(void);
void prompt(void);

int parameter = 0;
int keypress = 0;

#define KEY_NONE 0

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

void drive_column(int column, int state)
{
  int gpio = col[column];

  //printf("\nDriving %d to %d", col[column], state);
  
  gpio_put(gpio, state);
}

void drive_row(int row)
{
#if 0
  switch(row)
    {
    case 0:
      gpio_put(PIN_MUX_A, 0);
      gpio_put(PIN_MUX_B, 0);
      gpio_put(PIN_MUX_C, 0);
      break;

    case 1:
      gpio_put(PIN_MUX_A, 1);
      gpio_put(PIN_MUX_B, 0);
      gpio_put(PIN_MUX_C, 0);
      break;

    case 2:
      gpio_put(PIN_MUX_A, 0);
      gpio_put(PIN_MUX_B, 1);
      gpio_put(PIN_MUX_C, 0);
      break;
    
    case 3:
      gpio_put(PIN_MUX_A, 1);
      gpio_put(PIN_MUX_B, 1);
      gpio_put(PIN_MUX_C, 0);
      break;
    }
#else

#if 1
  gpio_put(PIN_MUX_A, (row & 1)>>0);
  gpio_put(PIN_MUX_B, (row & 2)>>1);
  gpio_put(PIN_MUX_C, (row & 4)>>2);
#else
  gpio_put(PIN_MUX_A, (parameter & 1)>>0);
  gpio_put(PIN_MUX_B, (parameter & 2)>>1);
  gpio_put(PIN_MUX_C, (parameter & 4)>>2);
#endif
#endif

  //printf("\nSelecting row %d", row);
}

void cli_scan(void)
{
  int mat[NUM_COL][NUM_ROW];
  int done = 0;
  
  while(!done)
    {
      int pressed;

      pressed = 0;
      
      for(int c=0; c< NUM_COL; c++)
	{
	  drive_column(c, 1);
	}

      sleep_ms(1);
      
  for(int c=0; c< NUM_COL; c++)
    {
      drive_column(c, 0);
      sleep_ms(1);
      
      for(int r=0; r<NUM_ROW; r++)
	{
	  drive_row(r);
	  sleep_ms(1);
	  int v = adc_read();
	  v = adc_read();
	  
	  mat[c][r] = v;
	  if( v < 1000 )
	    {
	      pressed = 1;
	    }
	}
      drive_column(c, 1);
    }
  
  if( !pressed )
    {
      continue;
    }
    
  for(int c=0; c< NUM_COL; c++)
    {
      printf("\n");
      
      drive_column(c, 0);

      for(int r=0; r<NUM_ROW; r++)
	{
	  printf("%4d (%d,%d)       ", mat[c][r], c, r);	  
	}
      drive_column(c, 1);
    }

  printf("\n");
  int pc = -1;
  int pr = -1;
  
  for(int c=0; c< NUM_COL; c++)
    {
      printf("\n");
      
      drive_column(c, 0);
      for(int r=0; r<NUM_ROW; r++)
	{

	  if( mat[c][r] < 1000 )
	    {
	      printf("*  ");
	      pc = c;
	      pr = r;
	    }
	  else
	    {
	      printf("   ");
	    }
	}
      drive_column(c, 1);
    }
  //done = 1;
  printf("\n(%d, %d)", pc, pr);
    }
  
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
    'R',
    "Drive row",
    cli_drive_row,
   },
   {
    'C',
    "Drive column",
    cli_drive_column,
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
   
  };

//------------------------------------------------------------------------------


void serial_loop(void)
{
  int key = KEY_NONE;

  
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

