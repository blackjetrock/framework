//

#define NUM_LEDS 24


typedef struct
{
  int cs;
  int sw;
} SINGLE_LED_INFO;

typedef struct
{
  SINGLE_LED_INFO r;
  SINGLE_LED_INFO g;
  SINGLE_LED_INFO b;
} RGB_LED_INFO;

#define CS_SW_TO_OFFSET(CS,SW) ((SW-1)*0x12+(CS-1))


void set_led_rgb(int x, int y, int r, int g, int b);
void set_page0_rgb(int x, int r, int g, int b);
