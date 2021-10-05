#ifndef CONFIG_H
#define CONFIG_H

// Set this to true to send debug output to the serial port
// Warning: This dramatically slows down processing of inputs
const bool DEBUG = false;

// LED Brightnesses (in percent)
#define LED_IDLE_BRIGHTNESS    1
#define LED_ACTIVE_BRIGHTNESS 80

/* Key bindings

This keybindings array lets you define what you want buttons to do, you can choose up to 4 buttons to press
(in the order specified) and then whether to BUTTON_PRESS_HOLD, BUTTON_RAPID_FIRE or BUTTON_TAP_ON_RELEASE.

Releasing a button will release them in reverse order.

BUTTON_PRESS_HOLD = Acts like a regular keyboard button, holding it down may engage keyboard auto-repeat.
BUTTON_RAPID_FIRE = Rapidly presses and releases the keystrokes as if you were mashing the button yourself.
BUTTON_TAP_ON_RELEASE = Does nothing on button press but on release it will send the defined key strokes.

Regular keystrokes are in the format of char('a'), char('1') and char('/') and assume a US keyboard.
Modifier keys that don't type a specific character can be found in non_ascii_keys.h or on this wiki page;
https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/

If you do not want a button specified at any particular stage you *MUST* set it to KEY_NONE.

Examples:

To send the number 1 that behaves like a regular keyboard button, you would use the following;

  { char('1'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD },

To send CTRL+ALT+DELETE in the same way you would use the following;

  { KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_DELETE, KEY_NONE, BUTTON_PRESS_HOLD },

To auto-repeat pressing the page down button you would use the following;

  { KEY_PAGE_DOWN, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_RAPID_FIRE },

To only send the combination when the button is released, use BUTTON_TAP_ON_RELEASE.
This is primarily used for debouncing the D-PAD buttons but you can use it elsewhere if you desire.

*/

const uint8_t keybindings[34][5] = {
  // Top row of toggle switches
  { char('r'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button A
  { char('u'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button B
  { char('i'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button C
  { char('o'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button D
  { char('p'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button E
  { char('['), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button F

  // Bottom row of toggle switches
  { KEY_LEFT_CTRL, char('u'), KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button G
  { KEY_LEFT_CTRL, char('u'), KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button H
  { KEY_LEFT_CTRL, char('i'), KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button I
  { KEY_LEFT_CTRL, char('o'), KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button J
  { KEY_LEFT_CTRL, char('p'), KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button K
  { char(']'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button L

  // Top row of square buttons
  { KEY_HASH, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button M
  { KEY_F11, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button N
  { KEY_F1, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button O
  { KEY_F2, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button P
  { char('i'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button Q
  { char('y'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button R

  // Bottom row of square buttons
  { char('n'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button S
  { char('m'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button T
  { char('l'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button U
  { KEY_SEMICOLON, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button V
  { KEY_QUOTE, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button W
  { char('k'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_PRESS_HOLD }, // Button X

  // All of the following buttons must be BUTTON_TAP_ON_RELEASE for buttons 5 and 10 to work

  // Left D-PAD directional
  { char('7'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 1
  { KEY_LEFT_CTRL, char('7'), KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 2
  { KEY_LEFT_CTRL, char('6'), KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 3
  { char('6'), KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 4

  // Right D-PAD directional
  { KEYPAD_8, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 6
  { KEYPAD_2, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 7
  { KEYPAD_4, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 8
  { KEYPAD_6, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 9

  // Left and Right D-PAD centre push
  { KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 5
  { KEYPAD_5, KEY_NONE, KEY_NONE, KEY_NONE, BUTTON_TAP_ON_RELEASE }, // Button 10
};

#endif // CONFIG_H
