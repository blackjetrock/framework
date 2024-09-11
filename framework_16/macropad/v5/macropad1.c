////////////////////////////////////////////////////////////////////////////////
//
// Macropad features
//
// Things to do with the top level aspect of being a macropad
//
//
// There's an initialisation followed by a main loop
//
// Whatever is needed is set up and then processed in the loop.
//
//
////////////////////////////////////////////////////////////////////////////////

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

#include "macropad.h"
#include "keyboard.h"
#include "serial.h"
#include "i2c.h"
#include "led.h"
#include "sleep.h"

// Blink pattern
enum
  {
    BLINK_NOT_MOUNTED = 250, // device not mounted
    BLINK_MOUNTED = 1000,    // device mounted
    BLINK_SUSPENDED = 2500,  // device is suspended
  };

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;


////////////////////////////////////////////////////////////////////////////////
//
// GPIOs
//
//
////////////////////////////////////////////////////////////////////////////////

const uint PIN_SLEEP      = 0;
const uint PIN_MUX_A      = 1;
const uint PIN_MUX_B      = 2;
const uint PIN_MUX_C      = 3;
const uint PIN_MUX_ENABLE = 4;
const uint PIN_UNUSED1    = 5;
const uint PIN_UNUSED2    = 6;

const uint PIN_KD0        = 8;
const uint PIN_KD1        = 9;
const uint PIN_KD2        = 10;
const uint PIN_KD3        = 11;
const uint PIN_KD4        = 12;
const uint PIN_KD5        = 13;
const uint PIN_KD6        = 14;
const uint PIN_KD7        = 15;
const uint PIN_KD13       = 16;
const uint PIN_KD12       = 17;
const uint PIN_KD11       = 18;
const uint PIN_KD10       = 19;
const uint PIN_KD9        = 20;
const uint PIN_KD8        = 21;
const uint PIN_KD15       = 22;
const uint PIN_KD14       = 23;

const uint PIN_ADC        = 28;
const uint PIN_SDB        = 29;

const int col[NUM_COL] =
  {
    PIN_KD0,
    PIN_KD1,
    PIN_KD2,
    PIN_KD3,
    PIN_KD4,
    PIN_KD5,
    PIN_KD6,
    PIN_KD7,
    PIN_KD8,
    PIN_KD9,
    PIN_KD10,
    PIN_KD11,
    PIN_KD12,
    PIN_KD13,
    PIN_KD14,
    PIN_KD15,
  };

void gpio_set_out(int g)
{
  gpio_init(g);
  gpio_set_dir(g, GPIO_OUT);
  //printf("\nSetting gpio %d to output", g);
}

void gpio_set_input(int g)
{
  gpio_init(g);
  gpio_set_dir(g, GPIO_IN);
}

void init_gpios(void)
{
  for(int i=0; i<NUM_COL; i++)
    {
      gpio_set_out(col[i]);
    }

  gpio_set_out(PIN_MUX_ENABLE);
  gpio_put(PIN_MUX_ENABLE, 0);

  // SDB on the LED driver needs to be high to turn LEDs off
  gpio_set_out(PIN_SDB);
  gpio_put(PIN_SDB, 1);
  
  gpio_set_out(PIN_MUX_A);
  gpio_set_out(PIN_MUX_B);
  gpio_set_out(PIN_MUX_C);

  gpio_set_input(PIN_UNUSED1);
  gpio_set_input(PIN_UNUSED2);

  adc_init();
  adc_gpio_init(PIN_ADC);
  adc_select_input(2);
  gpio_pull_up(PIN_ADC);

  
}

////////////////////////////////////////////////////////////////////////////////

void init_i2c(void)
{
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

}

////////////////////////////////////////////////////////////////////////////////
//
// Functions on the arrow keys.
//

int arrow_keys[24] =
  {
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_PAGE_UP,
    HID_KEY_PAGE_DOWN,
    
    HID_KEY_HOME,
    HID_KEY_PAGE_UP,
    HID_KEY_PAGE_DOWN,
    HID_KEY_END,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_NONE,           //16
    HID_KEY_ARROW_UP,
    HID_KEY_NONE,
    HID_KEY_NONE,
    HID_KEY_ARROW_LEFT,     //20
    HID_KEY_ARROW_DOWN,
    HID_KEY_ARROW_RIGHT,
    HID_KEY_NONE,           //23
  };


int macro_keys[24] =
  {
    HID_KEY_A,
    HID_KEY_B,
    HID_KEY_PAGE_UP,
    HID_KEY_PAGE_DOWN,
    HID_KEY_C,
    HID_KEY_D,
    HID_KEY_E,
    HID_KEY_F,
    HID_KEY_G,
    HID_KEY_H,
    HID_KEY_I,
    HID_KEY_J,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
    HID_KEY_A,
  };

////////////////////////////////////////////////////////////////////////////////
//
//

// Light key in blue
void light_key_b(int k)
{
  if( k != MATRIX_KEY_NONE )
    {
      //      deliver_key(macro_keys[k]);
      set_led_rgb(k, 0, 0x00, 0x00, 0x10);
      
    }
}

// Light key in green
void light_key_g(int k)
{
  if( k != MATRIX_KEY_NONE )
    {
      //      deliver_key(macro_keys[k]);
      set_led_rgb(k, 0, 0x00, 0x10, 0x00);
      
    }
}

// Light key in red
void light_key_r(int k)
{
  if( k != MATRIX_KEY_NONE )
    {
      //      deliver_key(macro_keys[k]);
      set_led_rgb(k, 0, 0x10, 0x00, 0x00);
      
    }
}

void init_null(void)
{
}

// A mode that generates the fn shifted arrow keys

void arrow_mode_setup(void)
{
  clr_led();

  // Home, page up, page down, end
  set_led_rgb(5, 0, 0x20, 0x00, 0x20);
  set_led_rgb(6, 0, 0x20, 0x00, 0x20);
  set_led_rgb(4, 0, 0x00, 0x10, 0x00);
  set_led_rgb(7, 0, 0x00, 0x10, 0x00);

  // Arrow keys
  set_led_rgb(17, 0, 0x00, 0x00, 0x10);
  set_led_rgb(20, 0, 0x00, 0x00, 0x10);
  set_led_rgb(21, 0, 0x00, 0x00, 0x10);
  set_led_rgb(22, 0, 0x00, 0x00, 0x10);

}

void arrow_mode(int k)
{
  deliver_key(arrow_keys[k]);
}

int current_mode = 0;

typedef void (*MODE_PTR)(int k);

MODE_PTR mode_ptr[] =
  {
    arrow_mode,
    light_key_r,
    light_key_g,
    light_key_b,
  };

typedef void (*INIT_PTR)(void);

INIT_PTR init_ptr[] =
  {
    arrow_mode_setup,
    init_null,
    init_null,
    init_null,
  };

char *desc_ptr[] =
  {
    "Mode:unshifted arrow functions",
    "Mode:Pressed keys turn on red   LED",
    "Mode:Pressed keys turn on green LED",
    "Mode:Pressed keys turn on blue  LED",
  };

#define NUM_MODES (sizeof(mode_ptr) / sizeof(MODE_PTR))

void mode_handle_key(int k)
{
  (*mode_ptr[current_mode])(k);
}

////////////////////////////////////////////////////////////////////////////////

void oledmain(void);
void serial_loop(void);

int main() {
  
  i2c_init_il31();
  
  init_keyboard();
  stdio_init_all();


  init_usb();
  init_gpios();
  
  sleep_ms(300);
      
  // Main loop
  int loop_count = 0;
  int sent_banner = 0;

  (*init_ptr[current_mode])();
  printf("\n\n%s", desc_ptr[current_mode]);

  while(1)
    {
      // Check for sleep. If low then go to sleep
      if( gpio_get(PIN_SLEEP) )
	{
	  // No need to go to sleep
	}
      else
	{
	  printf("\nSleeping...");
	  
	  // Turn LEDs off
	  gpio_put(PIN_SDB, 0);

	  // Enter sleep mode, with a wakeup on sleep going high again
	  // so set up for edge triggered and high (rising edge)
	  sleep_goto_dormant_until_pin(PIN_SLEEP, 1, 1);

	  gpio_put(PIN_SDB, 1);
	  printf("\nWoken up");
	}
      
      loop_count++;

      if( loop_count == 2500 )
	{
	  if( !sent_banner )
	    {
	      printf("\n\nMacropad CDC Stdio\n\n");
	      serial_help();
	      sent_banner = 1;
	    }
	}
      
      tud_task();
      key_scan();
      serial_loop();
      hid_task();

      char k = nos_get_key();

      // Key code 0 cycles through the different modes
      switch(k)
	{
	case 0:
	  current_mode = (current_mode+1) % NUM_MODES;
	  (*init_ptr[current_mode])();
	  printf("\n\n%s", desc_ptr[current_mode]);
	  break;

	case 255:
	  break;
	  
	default:
	  mode_handle_key(k);
	  break;
	}
    }
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+
#if 0
uint8_t const desc_hid_report[] =
  {
    TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD         )),
    TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE            )),
    TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL )),
    TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(REPORT_ID_GAMEPAD          ))
  };
#endif

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
  (void) instance;
  return desc_hid_report;
}


#if 0
// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
{
  // not implemented, we only send REPORT_ID_KEYBOARD
  (void)instance;
  (void)len;
}
#endif

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Keys are reported here
// We have access functions for delivering the key presses

#define KEY_PRESSED_TICKS  2
#define KEY_RELEASED_TICKS  3

#define KEY_PRESS_CYCLE_TICKS  (KEY_PRESSED_TICKS+KEY_RELEASED_TICKS)

int key_being_sent = 0;
int key_to_send = HID_KEY_NONE;

int keycnt = 0;
int key = HID_KEY_NONE;

//------------------------------------------------------------------------------
// New key to be sent
// 0: not sent
// 1: sent

int deliver_key(int key_to_deliver)
{
  int ret = 0;

  //printf("\n%s:%d", __FUNCTION__, key_to_deliver);
  
  if( deliver_key_done() )
    {
      // Send new key
      key_to_send = key_to_deliver;
      
      key_being_sent = 1;
      ret = 1;
    }

  return(ret);
}

int deliver_key_done(void)
{
  //printf("\n%s:%d", __FUNCTION__, !key_being_sent);
  return(!key_being_sent);
}


//------------------------------------------------------------------------------
//
// Called every 10ms
//

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
#if 0
  if( key_being_sent )
    {
      keycnt++;  
      
      switch(keycnt)
	{
	case 1:
	  printf("\n%s:1", __FUNCTION__);
	  key = key_to_send;
	  break;
	  
	  // Key has been pressed, now release it for a bit
	case KEY_PRESSED_TICKS:
	  printf("\n%s:2", __FUNCTION__);
	  key = HID_KEY_NONE;
	  break;
	  
	case KEY_PRESS_CYCLE_TICKS:
	  keycnt = 0;
	  key_being_sent = 0;
	  printf("\n%s:3", __FUNCTION__);
	  break;
	  
	default:
	  break;
	}
    }
#endif
  
  // skip if hid is not ready yet
  if ( !tud_hid_ready() )
    return;
  
  switch(report_id)
    {
    case REPORT_ID_KEYBOARD:
#if 0
      {

	// use to avoid send multiple consecutive zero report for keyboard
	static bool has_keyboard_key = false;

	if ( btn,1 )
	  {
	    uint8_t keycode[6] = { 0 };
	    keycode[0] = key;

	    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
	    key = HID_KEY_NONE;
	    has_keyboard_key = true;
	  }
	else
	  {
	    // send empty key report if previously has key pressed
	    if (has_keyboard_key)
	      {
		tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
	      }
	    has_keyboard_key = false;
	  }
      }
#endif

      if( key_being_sent && (key_to_send != MATRIX_KEY_NONE) )
	{
	  uint8_t keycode[6] = { 0 };
	  keycode[0] = key_to_send;
	  
	  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
	}
      else
	{
	  tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
	}
      
      key_being_sent = 0;
      break;

    case REPORT_ID_MOUSE:
      {
	int8_t const delta = 5;

	// no button, right + down, no scroll, no pan
	tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
      }
      break;

    case REPORT_ID_CONSUMER_CONTROL:
      {
	// use to avoid send multiple consecutive zero report
	static bool has_consumer_key = false;

	if ( btn )
	  {
	    // volume down
	    uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
	    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
	    has_consumer_key = true;
	  }else
	  {
	    // send empty key report (release key) if previously has key pressed
	    uint16_t empty_key = 0;
	    if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
	    has_consumer_key = false;
	  }
      }
      break;

    case REPORT_ID_GAMEPAD:
      {
	// use to avoid send multiple consecutive zero report for keyboard
	static bool has_gamepad_key = false;

	hid_gamepad_report_t report =
	  {
	    .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
	    .hat = 0, .buttons = 0
	  };

	if ( btn )
	  {
	    report.hat = GAMEPAD_HAT_UP;
	    report.buttons = GAMEPAD_BUTTON_A;
	    tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

	    has_gamepad_key = true;
	  }else
	  {
	    report.hat = GAMEPAD_HAT_CENTERED;
	    report.buttons = 0;
	    if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
	    has_gamepad_key = false;
	  }
      }
      break;

    default: break;
    }
}


////////////////////////////////////////////////////////////////////////////////
//
// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
//
////////////////////////////////////////////////////////////////////////////////


void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  uint32_t const btn = board_button_read();

  // Remote wakeup
  if ( tud_suspended() && btn )
    {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host
      tud_remote_wakeup();
    }
  else
    {
      // Send the 1st of report chain, the rest will be sent by tud_hid_report_complete_cb()
      send_hid_report(REPORT_ID_KEYBOARD, btn);
    }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  uint8_t next_report_id = report[0] + 1u;

  if (next_report_id < REPORT_ID_COUNT)
    {
      send_hid_report(next_report_id, board_button_read());
    }
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void)instance;
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
  (void)instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
      // Set keyboard LED e.g Capslock, Numlock etc...
      if (report_id == REPORT_ID_KEYBOARD)
        {
	  // bufsize should be (at least) 1
	  if (bufsize < 1)
	    return;

	  uint8_t const kbd_leds = buffer[0];

	  if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
	      // Capslock On: disable blink, turn led on
	      blink_interval_ms = 0;
	      //	      board_led_write(true);
            }
	  else
            {
	      // Caplocks Off: back to normal blink
	      board_led_write(false);
	      //blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void)remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

