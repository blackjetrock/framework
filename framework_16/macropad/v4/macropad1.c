////////////////////////////////////////////////////////////////////////////////

#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

#include "macropad.h"

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

const uint PIN_MUX_ENABLE = 4;
const uint PIN_MUX_A      = 1;
const uint PIN_MUX_B      = 2;
const uint PIN_MUX_C      = 3;

const uint PIN_UNUSED1  = 5;
const uint PIN_UNUSED2  = 6;

const uint PIN_KD0  = 8;
const uint PIN_KD1  = 9;
const uint PIN_KD2  = 10;
const uint PIN_KD3  = 11;
const uint PIN_KD4  = 12;
const uint PIN_KD5  = 13;
const uint PIN_KD6  = 14;
const uint PIN_KD7  = 15;
const uint PIN_KD8  = 21;
const uint PIN_KD9  = 20;
const uint PIN_KD10 = 19;
const uint PIN_KD11 = 18;
const uint PIN_KD12 = 17;
const uint PIN_KD13 = 16;
const uint PIN_KD14 = 23;
const uint PIN_KD15 = 22;

const uint PIN_ADC  = 28;


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
  printf("\nSetting gpio %d to output", g);
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

  gpio_set_out(29);
  gpio_put(29, 1);
  
  gpio_set_out(PIN_MUX_A);
  gpio_set_out(PIN_MUX_B);
  gpio_set_out(PIN_MUX_C);

  gpio_set_input(PIN_UNUSED1);
  gpio_set_input(PIN_UNUSED2);
  
}

////////////////////////////////////////////////////////////////////////////////

void init_i2c(void)
{
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

}

////////////////////////////////////////////////////////////////////////////////

void oledmain(void);
void serial_loop(void);

int main() {



  
  stdio_init_all();
  sleep_ms(2000);
  
  adc_init();
  adc_gpio_init(PIN_ADC);
  adc_select_input(2);
  gpio_pull_up(PIN_ADC);
  init_gpios();

  
  printf("\nMacropad CDC Stdio");

  init_usb();
    
  // Main loop
  while(1)
    {

      tud_task();
      serial_loop();
      //hid_task();
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

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn,1 )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
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

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
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
  }else
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
                board_led_write(true);
            }
            else
            {
                // Caplocks Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
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

