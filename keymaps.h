#ifndef keymaps_h
#define keymaps_h

#include <stdint.h>
#include "usb_hid.h"

// button to keycode mapping
// button 1-8 mapped to whatever u want

typedef struct {
    uint8_t keycode;
    uint8_t modifier;
} keymap_entry_t;

// default keymap - can change this later
// button 1-8 basic mapping
static const keymap_entry_t default_keymap[8] = {
    {HID_KEY_F13, 0},        // button 1 - F13
    {HID_KEY_F14, 0},        // button 2 - F14
    {HID_KEY_F15, 0},        // button 3 - F15
    {HID_KEY_F16, 0},        // button 4 - F16
    {HID_KEY_F17, 0},        // button 5 - F17
    {HID_KEY_F18, 0},        // button 6 - F18
    {HID_KEY_F19, 0},        // button 7 - F19
    {HID_KEY_F20, 0},        // button 8 - F20
};

// get the key for a button
keymap_entry_t keymap_get(uint8_t button_id);

#endif
