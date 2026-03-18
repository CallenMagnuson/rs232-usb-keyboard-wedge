# rs232-usb-keyboard-wedge
An RS-232 to USB Keyboard wedge for typing keyboard shortcuts via serial control
Made for the Arduino Leonardo and other compatible ATMega32U4 microcontrollers. Pin references are for Arduino pinouts.

Accepts input in the ASCII character space and will do its best to type those keys.

This was designed to use as an RS-232 to USB HID Keyboard wedge to type keyboard shortcuts on another device.

Unused ASCII space is used for modifier keys with the following keys defined:
CTRL (left ctrl): Decimal 129 - Hex 81
ALT (left alt): Decimal 141 - Hex 8D
OS Key (left Windows/OS): Decimal 143 - Hex 8F
Shift (left shift): Decimal 144 - Hex 90
Tab: Decimal 157 - Hex 9D

Other ASCII characters will be typed as received.
