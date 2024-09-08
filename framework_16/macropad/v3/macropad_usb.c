
#include "macropad.h"

#include "bsp/board_api.h"
#include "tusb.h"

////////////////////////////////////////////////////////////////////////////////




void init_usb(void)
{
  //    tusb_init();
  board_init();
  
  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);
  
  if (board_init_after_tusb) {
    board_init_after_tusb();
  }
  
}
