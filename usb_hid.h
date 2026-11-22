#ifndef usb_hid_h
#define usb_hid_h

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "tusb.h"

// hid keyboard stuff (not using anymore, switched to CDC)
// keeping this file around in case we want HID later

void usb_hid_init(void);
void usb_hid_task(void);
bool usb_hid_is_ready(void);

// send key stuff
void usb_hid_send_key(uint8_t keycode);
void usb_hid_send_key_with_mod(uint8_t keycode, uint8_t modifier);
void usb_hid_release_all(void);

// modifiers
#define HID_MOD_LCTRL 0x01
#define HID_MOD_LSHIFT 0x02
#define HID_MOD_LALT 0x04
#define HID_MOD_LMETA 0x08
#define HID_MOD_RCTRL 0x10
#define HID_MOD_RSHIFT 0x20
#define HID_MOD_RALT 0x40
#define HID_MOD_RMETA 0x80

// F1-F12 and F13-F18 are defined in TinyUSB's hid.h
// via tusb.h include above

// some extra keycodes we might need
#define HID_KEY_NONE 0x00

#endif
