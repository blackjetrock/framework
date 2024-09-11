Framework 16

*** Work in progress ***

RGB Macropad Firmware

![IMG_20240910_215325760](https://github.com/user-attachments/assets/a20ad5b8-39f2-4137-b010-67167023473f)

This firmware is based on TinyUSB and does not use QMK. It is hacked together and not finished. It should be simple to extend or change as required.
The code presents a HID keyboard, a mass storage device (there because the code I based this on had it) and a USB serial port. The USB serial has a menu driven interface that is used to debiug the code at the moment, but could be used for configuration.

Putting macropad into mass storage mode.
This can be done in the serial CLI of the firmware, or if the mode needs to be forced due to bad code, then connecting TP9 and TP13 through a 2k2 resistor works. I have wired a switch and resistor on flying leads.

![IMG_20240910_215949436_HDR](https://github.com/user-attachments/assets/fb47f64d-cf3e-4d0f-a3de-08ecaf15d2c9)

Video here of keys being pressed and LEDs being lit.

https://www.youtube.com/watch?v=P2B2t0fApKs
