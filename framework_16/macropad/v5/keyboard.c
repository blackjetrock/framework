

#include "macropad.h"


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

void key_scan(void)
{
  int mat[NUM_COL][NUM_ROW];
  int done = 0;
  int pressed;
 
  pressed = 0;
  while(1)
    {
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

  if( !pressed,0 )
    {
      return;
    }

  printf("\nPressed");
  
  //------------------------------------------------------------------------------
  // A key has been pressed
    
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
