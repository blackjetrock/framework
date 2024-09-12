# Framework 16

# *** Work in progress ***

## RGB Macropad Firmware

<img src="https://github.com/user-attachments/assets/a20ad5b8-39f2-4137-b010-67167023473f" width="300"/>

This firmware is based on TinyUSB and does not use QMK. It is hacked together and not finished. It should be simple to extend or change as required.
The code presents a HID keyboard, a mass storage device (there because the code I based this on had it) and a USB serial port. The USB serial has a menu driven interface that is used to debiug the code at the moment, but could be used for configuration.

Putting macropad into mass storage mode.
This can be done in the serial CLI of the firmware, or if the mode needs to be forced due to bad code, then connecting TP9 and TP13 through a 2k2 resistor works. I have wired a switch and resistor on flying leads.

<img src="https://github.com/user-attachments/assets/fb47f64d-cf3e-4d0f-a3de-08ecaf15d2c9" width="300"/>

## Default Modes
I have some code that sets up modes I want. You can add whatever you need. 

The top left key cycles through the modes.

### Initial mode

<img src="https://github.com/user-attachments/assets/c4d863d0-c671-4f58-890f-0a276b3eb132" width="300"/>

Sets up LEDs and keys:

  * Green:  HOME and END
  * Purple: PAGE UP and PAGE DOWN
  * Blue:   LEFT, RIGHT, UP, DOWN
  * Cyan:   minicom -D /dev/ttyACM0

### Number Mode

![IMG_20240912_065953683](https://github.com/user-attachments/assets/b7caa5f4-c162-405c-bea7-faf21405c396)

This mode sends incrementing numbers in hex or decimal, with or without spaces

* Green:   Hex, trailing space
* Purple:  Decimal, no space
* Red:     Decimal, trailing space
  

## Videos

https://www.youtube.com/watch?v=9jJmbEcvwuo&t=39s
