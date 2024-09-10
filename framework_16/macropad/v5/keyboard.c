

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

#include "macropad.h"

#include "keyboard.h"


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
// This allows other code to insert keys into the key queue. Note: Core1
// has to be the code that does this, and i tmust run on core1. So, put a key in this
// location and wait for it to tuirn to KEY_NONE before putting another one there.
// If multiple code needs ot do this, then add more variables and add insertion in the KB code.

KEYCODE kb_external_key = MATRIX_KEY_NONE;

// Called regularly, can be on core1.

// Last time a key was presed (or we reset). used to turn off after some time delay
uint64_t last_key_press_time = 0;
int init_last_key = 1;

// How long we wait before turning off if no keys pressed
uint64_t kb_inactivity_timeout = 20*60*1000*1000;
uint64_t last_tick_time = 0;
uint64_t tick_interval = 5*1000*1000;
int tick = 0;

// Bit positions within matrix of each key

#define MATRIX_BIT_CAP    1
#define MATRIX_BIT_NUM    2
#define MATRIX_BIT_UP     1
#define MATRIX_BIT_DOWN   2
#define MATRIX_BIT_LEFT   3
#define MATRIX_BIT_RIGHT  4
#define MATRIX_BIT_A      9
#define MATRIX_BIT_B     14
#define MATRIX_BIT_C     19
#define MATRIX_BIT_D     34
#define MATRIX_BIT_E     24
#define MATRIX_BIT_F     29
#define MATRIX_BIT_G      8
#define MATRIX_BIT_H     13
#define MATRIX_BIT_I     18
#define MATRIX_BIT_J     33
#define MATRIX_BIT_K     23
#define MATRIX_BIT_L     28
#define MATRIX_BIT_M      7
#define MATRIX_BIT_N     12
#define MATRIX_BIT_O     17
#define MATRIX_BIT_P     32
#define MATRIX_BIT_Q     22
#define MATRIX_BIT_R     27
#define MATRIX_BIT_S      6
#define MATRIX_BIT_T     11
#define MATRIX_BIT_U     16
#define MATRIX_BIT_V     31
#define MATRIX_BIT_W     21
#define MATRIX_BIT_X     26
#define MATRIX_BIT_Y     15
#define MATRIX_BIT_Z     30
#define MATRIX_BIT_EXE   25
#define MATRIX_BIT_SPACE 20
#define MATRIX_BIT_MODE   0
#define MATRIX_BIT_DEL   10

#define MATRIX_BIT_SHIFT  5
#define MATRIX_BIT_ON    63


volatile MATRIX_MAP mat_scan_matrix = 0;

typedef struct _KEYMAP
{
  uint64_t mask;
  char c;
} KEYMAP;

KEYMAP base_map[] =
  {
    { (uint64_t)1 << MATRIX_BIT_ON,     1 },
    { (uint64_t)1 << MATRIX_BIT_UP,     3 },
    { (uint64_t)1 << MATRIX_BIT_DOWN,   4 },
    { (uint64_t)1 << MATRIX_BIT_LEFT,   5 },
    { (uint64_t)1 << MATRIX_BIT_RIGHT,  6 },
    { (uint64_t)1 << MATRIX_BIT_A,     'A' },
    { (uint64_t)1 << MATRIX_BIT_B,     'B' },
    { (uint64_t)1 << MATRIX_BIT_C,     'C' },
    { (uint64_t)1 << MATRIX_BIT_D,     'D' },
    { (uint64_t)1 << MATRIX_BIT_E,     'E' },
    { (uint64_t)1 << MATRIX_BIT_F,     'F' },
    { (uint64_t)1 << MATRIX_BIT_G,     'G' },
    { (uint64_t)1 << MATRIX_BIT_H,     'H' },
    { (uint64_t)1 << MATRIX_BIT_I,     'I' },
    { (uint64_t)1 << MATRIX_BIT_J,     'J' },
    { (uint64_t)1 << MATRIX_BIT_K,     'K' },
    { (uint64_t)1 << MATRIX_BIT_L,     'L' },
    { (uint64_t)1 << MATRIX_BIT_M,     'M' },
    { (uint64_t)1 << MATRIX_BIT_N,     'N' },
    { (uint64_t)1 << MATRIX_BIT_O,     'O' },
    { (uint64_t)1 << MATRIX_BIT_P,     'P' },
    { (uint64_t)1 << MATRIX_BIT_Q,     'Q' },
    { (uint64_t)1 << MATRIX_BIT_R,     'R' },
    { (uint64_t)1 << MATRIX_BIT_S,     'S' },
    { (uint64_t)1 << MATRIX_BIT_T,     'T' },
    { (uint64_t)1 << MATRIX_BIT_U,     'U' },
    { (uint64_t)1 << MATRIX_BIT_V,     'V' },
    { (uint64_t)1 << MATRIX_BIT_W,     'W' },
    { (uint64_t)1 << MATRIX_BIT_X,     'X' },
    { (uint64_t)1 << MATRIX_BIT_Y,     'Y' },
    { (uint64_t)1 << MATRIX_BIT_Z,     'Z' },
    { (uint64_t)1 << MATRIX_BIT_EXE,   13 },
    { (uint64_t)1 << MATRIX_BIT_SPACE, ' ' },
    { (uint64_t)1 << MATRIX_BIT_MODE,   2 },
    { (uint64_t)1 << MATRIX_BIT_DEL,    8 },
    { 0,                '-' }
  };

KEYMAP caps_map[] =
  {
    { (uint64_t)1 << MATRIX_BIT_ON,     1 },
    { (uint64_t)1 << MATRIX_BIT_UP,     3 },
    { (uint64_t)1 << MATRIX_BIT_DOWN,   4 },
    { (uint64_t)1 << MATRIX_BIT_LEFT,   5 },
    { (uint64_t)1 << MATRIX_BIT_RIGHT,  6 },
    { (uint64_t)1 << MATRIX_BIT_A,     'a' },
    { (uint64_t)1 << MATRIX_BIT_B,     'b' },
    { (uint64_t)1 << MATRIX_BIT_C,     'c' },
    { (uint64_t)1 << MATRIX_BIT_D,     'd' },
    { (uint64_t)1 << MATRIX_BIT_E,     'e' },
    { (uint64_t)1 << MATRIX_BIT_F,     'f' },
    { (uint64_t)1 << MATRIX_BIT_G,     'g' },
    { (uint64_t)1 << MATRIX_BIT_H,     'h' },
    { (uint64_t)1 << MATRIX_BIT_I,     'i' },
    { (uint64_t)1 << MATRIX_BIT_J,     'j' },
    { (uint64_t)1 << MATRIX_BIT_K,     'k' },
    { (uint64_t)1 << MATRIX_BIT_L,     'l' },
    { (uint64_t)1 << MATRIX_BIT_M,     'm' },
    { (uint64_t)1 << MATRIX_BIT_N,     'n' },
    { (uint64_t)1 << MATRIX_BIT_O,     'o' },
    { (uint64_t)1 << MATRIX_BIT_P,     'p' },
    { (uint64_t)1 << MATRIX_BIT_Q,     'q' },
    { (uint64_t)1 << MATRIX_BIT_R,     'r' },
    { (uint64_t)1 << MATRIX_BIT_S,     's' },
    { (uint64_t)1 << MATRIX_BIT_T,     't' },
    { (uint64_t)1 << MATRIX_BIT_U,     'u' },
    { (uint64_t)1 << MATRIX_BIT_V,     'v' },
    { (uint64_t)1 << MATRIX_BIT_W,     'w' },
    { (uint64_t)1 << MATRIX_BIT_X,     'x' },
    { (uint64_t)1 << MATRIX_BIT_Y,     'y' },
    { (uint64_t)1 << MATRIX_BIT_Z,     'z' },
    { (uint64_t)1 << MATRIX_BIT_EXE,    13 },
    { (uint64_t)1 << MATRIX_BIT_SPACE,  ' ' },
    { (uint64_t)1 << MATRIX_BIT_MODE,   2 },
    { (uint64_t)1 << MATRIX_BIT_DEL,    7 },
    { 0,                '-' }
  };

KEYMAP shifted_map[] =
  {
    { (uint64_t)1 << MATRIX_BIT_ON,     1 },
    { (uint64_t)1 << MATRIX_BIT_UP,     3 },
    { (uint64_t)1 << MATRIX_BIT_DOWN,   4 },
    { (uint64_t)1 << MATRIX_BIT_LEFT,   5 },
    { (uint64_t)1 << MATRIX_BIT_RIGHT,  6 },
    { (uint64_t)1 << MATRIX_BIT_A,     '<' },
    { (uint64_t)1 << MATRIX_BIT_B,     '>' },
    { (uint64_t)1 << MATRIX_BIT_C,     '(' },
    { (uint64_t)1 << MATRIX_BIT_D,     ')' },
    { (uint64_t)1 << MATRIX_BIT_E,     '%' },
    { (uint64_t)1 << MATRIX_BIT_F,     '/' },
    { (uint64_t)1 << MATRIX_BIT_G,     '=' },
    { (uint64_t)1 << MATRIX_BIT_H,     '"' },
    { (uint64_t)1 << MATRIX_BIT_I,     '7' },
    { (uint64_t)1 << MATRIX_BIT_J,     '8' },
    { (uint64_t)1 << MATRIX_BIT_K,     '9' },
    { (uint64_t)1 << MATRIX_BIT_L,     '*' },
    { (uint64_t)1 << MATRIX_BIT_M,     ',' },
    { (uint64_t)1 << MATRIX_BIT_N,     '$' },
    { (uint64_t)1 << MATRIX_BIT_O,     '4' },
    { (uint64_t)1 << MATRIX_BIT_P,     '5' },
    { (uint64_t)1 << MATRIX_BIT_Q,     '6' },
    { (uint64_t)1 << MATRIX_BIT_R,     '-' },
    { (uint64_t)1 << MATRIX_BIT_S,     ';' },
    { (uint64_t)1 << MATRIX_BIT_T,     ':' },
    { (uint64_t)1 << MATRIX_BIT_U,     '1' },
    { (uint64_t)1 << MATRIX_BIT_V,     '2' },
    { (uint64_t)1 << MATRIX_BIT_W,     '3' },
    { (uint64_t)1 << MATRIX_BIT_X,     '+' },
    { (uint64_t)1 << MATRIX_BIT_Y,     '0' },
    { (uint64_t)1 << MATRIX_BIT_Z,     '.' },
    { (uint64_t)1 << MATRIX_BIT_EXE,    13 },
    { (uint64_t)1 << MATRIX_BIT_SPACE,  ' ' },
    { (uint64_t)1 << MATRIX_BIT_MODE,   2 },
    { (uint64_t)1 << MATRIX_BIT_DEL,    7 },
    { 0,                '-' }
  };

// This table picksa key map from the current values
// of shift_pressed, caps_on and num_on
#define CAPS_ON    1
#define NUM_ON     2
#define SHIFT_ON   4
#define CAPS_OFF   0
#define NUM_OFF    0
#define SHIFT_OFF  0

typedef struct _KEY_MAP_MAP
{
  int modes;
  KEYMAP *map;  
} KEY_MAP_MAP;

KEY_MAP_MAP key_map_map[8] =
  {
    {SHIFT_OFF | CAPS_OFF| NUM_OFF,  base_map},
    {SHIFT_OFF | CAPS_OFF| NUM_ON,   shifted_map},
    {SHIFT_OFF | CAPS_ON | NUM_OFF,  caps_map},
    {SHIFT_OFF | CAPS_ON | NUM_ON,   shifted_map},
    {SHIFT_ON  | CAPS_OFF| NUM_OFF,  shifted_map},
    {SHIFT_ON  | CAPS_OFF| NUM_ON,   base_map},
    {SHIFT_ON  | CAPS_ON | NUM_OFF,  shifted_map},
    {SHIFT_ON  | CAPS_ON | NUM_ON,   base_map},
  };

#define NUM_MODES ((sizeof(key_map_map))/(sizeof(KEY_MAP_MAP)))
KEYMAP *key_map;

////////////////////////////////////////////////////////////////////////////////
//
// Debounces the pressed key matrix
//
// We have a stream of matrices. A 1 is a poressed key. we need to deboune them
// all, which we can do on a bit by bit basis, all at once as integers.
//
// We remove short presses, 
#define MAX_INPUT_QUEUE  4
#define MAX_PRESS_QUEUE  2

MATRIX_MAP  input_queue[MAX_INPUT_QUEUE];
MATRIX_MAP  pressed_queue[MAX_PRESS_QUEUE];
MATRIX_MAP  released_queue[MAX_PRESS_QUEUE];
MATRIX_MAP  pressed_edges;
MATRIX_MAP  released_edges;
MATRIX_MAP  key_states = 0LL;

int caps_mode     = 0;
int num_mode      = 0;
int shift_pressed = 0;

//------------------------------------------------------------------------------
// The key buffer 
// Press and release event are queued and then sent on
//
volatile char nos_key_buffer[NOS_KEY_BUFFER_LEN];

// Keys in and out pointers
volatile int nos_key_in  = 0;
volatile int nos_key_out = 0;

#define NEXT_KEY_IDX(IDX) ((IDX+1)%NOS_KEY_BUFFER_LEN)

void nos_put_key(char key)
{
  // Is buffer full?
  if( NEXT_KEY_IDX(nos_key_in) == nos_key_out )
    {
      // Full, drop the key
      // Original beeps here
      return;
    }

  nos_key_buffer[nos_key_in] = key;
  nos_key_in = NEXT_KEY_IDX(nos_key_in);
}

char nos_get_key(void)
{
  char k = MATRIX_KEY_NONE;
  
  if( nos_key_in == nos_key_out )
    {
      return(k);
    }

  k = nos_key_buffer[nos_key_out];

  nos_key_out = NEXT_KEY_IDX(nos_key_out);
  return(k);
}


//------------------------------------------------------------------------------
//
//
//
// The matrix of pressed keys is stored in a uint64_t.
// The key scanner supplies a matrix that the debouncer handles.
//


#define KEY_PRESSED(MAP,BITNO) ((MAP) & ((uint64_t)1 << BITNO))

void matrix_debounce(MATRIX_MAP matrix)
{
  //printf("\nInput:%016llX", matrix);
  
  // Move queue along and insert new value
  for(int i=1; i<MAX_INPUT_QUEUE;i++)
    {
      input_queue[i] = input_queue[i-1];
    }

  input_queue[0] = matrix;

  // Work out which keys are pressed and unpressed
  // shift the pressed and unpressed queues so a new entry
  // can be added
  
  for(int i=1; i<MAX_PRESS_QUEUE;i++)
    {
      pressed_queue[i]  = pressed_queue[i-1];
      released_queue[i] = released_queue[i-1];
    }
  
  pressed_queue[0]  = ~((uint64_t)0);
  released_queue[0] = ~((uint64_t)0);

  //
  // AND the input and also the negation of the input
  // This gives us a stream of pressed and released edges.
  // To be pressed, all the key bits in th einoput queue have to show
  // pressed. This is the debounce time
  
  for(int i=0; i<MAX_INPUT_QUEUE;i++)
    {
      // All keys held for max_queue samples
      pressed_queue[0] &= input_queue[i];
      released_queue[0] &= ~(input_queue[i]);
    }

  // Now find pressed edge events and released edge events
  // Now look for rising and falling edges in the pressed and unpressed
  // matrices. These are key events.
  
  pressed_edges  =  pressed_queue[0] & ~pressed_queue[1];
  released_edges = ~pressed_queue[0] &  pressed_queue[1];

  // If something pressed or unpressed
  if ( pressed_edges | released_edges)
    {
      printf("\nPedges:%016llX Redges:%016llX", pressed_edges, released_edges);

      //printf("\n");
      for(int i=0; i<64; i++)
	{
	  if( ((uint64_t)1<<i) & pressed_edges )
	    {
	      nos_put_key(i);
	      printf("\n  PRESS %d", i);
	    }
	  
	  if( ((uint64_t)1<<i) & released_edges )
	    {
	      nos_put_key(MATRIX_KEY_NONE);
	      printf("\nUNPRESS %d", i);
	    }
	}
    }

  // Now set and clear bits in the state matrix which will give the current
  // states of each key

  // Pressed keys
  key_states |= pressed_edges;

  // released keys
  key_states &= ~(released_edges);

  //printf("\n%016llX", key_states);

  // We now have the current states of the keys. 
  // We have pressed and released edges, so we can generate pressed and released
  // events, and also work out which key map to use from the states of the
  // SHIFT, NUM and CAPS keys.

  // The key queue is made up of pressed events only
#if 0
  shift_pressed = key_states & ((uint64_t)1<< MATRIX_BIT_SHIFT);
  
  // We need to handle the shift key for upper/lower case
  // and also the CAPS and NUM states
  
  if( KEY_PRESSED(pressed_edges,MATRIX_BIT_CAP) && shift_pressed)
    {
      caps_mode = !caps_mode;
      printf("\nCAPS lock:%d", caps_mode);
    }
  
  if( KEY_PRESSED(pressed_edges,MATRIX_BIT_NUM) && shift_pressed)
    {
      num_mode = !num_mode;
      printf("\nNUM lock:%d", num_mode);
    }

  // Find the key map to use
  int cur_modes = 0;
  if( shift_pressed )
    {
      cur_modes |= SHIFT_ON;
    }

  if( caps_mode )
    {
      cur_modes |= CAPS_ON;
    }

  if( num_mode )
    {
      cur_modes |= NUM_ON;
    }

  key_map = base_map;
  
  for(int k=0; k<NUM_MODES; k++)
    {
      if( cur_modes == key_map_map[k].modes )
	{
	  key_map = key_map_map[k].map;
	  break;
	}
    }

#endif
#if 0
  // Now generate pressed events and put them into a queue
  int i = 0;
  
  while( key_map[i].mask != 0 )
    {
      if( (key_map[i].mask) & pressed_edges )
	{
	  // Key pressed
	  printf("\nC:%d", key_map[i].c);

	  // Update inactivity timeout
	  last_key_press_time = time_us_64();
	  
	  // Put key pressed event into key buffer
	  nos_put_key(key_map[i].c);
	  return;
	}
      i++;
    }

  // We may have keys from other sources, accept those as well
  if( kb_external_key != KEY_NONE )
    {
      // Update inactivity timeout
      last_key_press_time = time_us_64();

      nos_put_key(kb_external_key);
      kb_external_key = KEY_NONE;
    }
#endif
#if 0
  // Inactivity processing

  // Initialise the last key time as global initialisation has to be constant
  if( init_last_key )
    {
      init_last_key = 0;
      last_key_press_time = time_us_64();
      last_tick_time =  last_key_press_time;
    }

  uint64_t now = time_us_64();
  uint64_t inactivity_time = (now - last_key_press_time);

  if( (now - last_tick_time) > tick_interval )
    {
      last_tick_time = now;
      tick = 1;
    }
#endif
 
}


////////////////////////////////////////////////////////////////////////////////
//
//  Key scan
//
//
////////////////////////////////////////////////////////////////////////////////

int key_grid_map[NUM_KEYS] =
  {
    17,
    18,
    52,
    20,
    32,
    36,
    1,
    6,
    33,
    37,
    2,
    21,
    34,
    38,
    3,
    7,
    35,
    39,
    4,
    22,
    0,
    23,
    5,
    16,
  };

int key_grid_reverse_map[NUM_COL*NUM_ROW];

void init_keyboard(void)
{
  // Build the key index map
  for(int i=0; i<NUM_KEYS; i++)
    {
      key_grid_reverse_map[key_grid_map[i]] = i;
    }
}


void key_scan(void)
{
  int mat[NUM_COL][NUM_ROW];
  int done = 0;
  int pressed;
 
  pressed = 0;
  //while(1)
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

      //printf("\nPressed");
  
      //------------------------------------------------------------------------------
      // A key has been pressed
#if 0
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
#endif
      
      int pc = -1;
      int pr = -1;

      MATRIX_MAP mm = 0;
      
      for(int c=0; c< NUM_COL; c++)
	{
	  //printf("\n");
      
	  drive_column(c, 0);
	  for(int r=0; r<NUM_ROW; r++)
	    {

	      if( mat[c][r] < 1000 )
		{
		  //	  printf("*  ");
		  pc = c;
		  pr = r;
		  
		  int key_index = key_grid_reverse_map[pr*16+pc];

		  mm |= 1 << (key_index);
		}
	      else
		{
		  //printf("   ");
		}
	    }
	  drive_column(c, 1);
	}

      //done = 1;
      //printf("\n(%d, %d)", pc, pr);

      // Create a matrix of keys and pass to the debounce function
      if( (pc == -1) && (pr == -1),0 )
	{
	}
      else
	{
	  MATRIX_MAP m;
	  
	  int key_index = key_grid_reverse_map[pr*16+pc];
	  
	  //printf("\nKey_index:%d", key_index);

	  // Send to debouncer
	  m = 1 << (key_index);
	  
	  matrix_debounce(mm);
	}
    }
}
