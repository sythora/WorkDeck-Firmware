#include "usb_hid.h"
#include "pico/stdlib.h"
#include <string.h>

// NOTE: this module is deprecated, we're using CDC now
// keeping the code here in case we need to switch back

static uint8_t keyboard_report[8] = {0};

void usb_hid_init(void) {
    // tusb_init() is called elsewhere now
}

void usb_hid_task(void) {
    tud_task();
}

bool usb_hid_is_ready(void) {
    return tud_hid_ready();
}

void usb_hid_send_key(uint8_t keycode) {
    usb_hid_send_key_with_mod(keycode, 0);
}

void usb_hid_send_key_with_mod(uint8_t keycode, uint8_t modifier) {
    if (!tud_hid_ready()) return;
    
    // build report
    keyboard_report[0] = modifier;
    keyboard_report[2] = keycode;
    
    tud_hid_report(0, keyboard_report, sizeof(keyboard_report));
    
    // small delay then release
    sleep_ms(10);
    usb_hid_release_all();
}

void usb_hid_release_all(void) {
    if (!tud_hid_ready()) return;
    
    memset(keyboard_report, 0, sizeof(keyboard_report));
    tud_hid_report(0, keyboard_report, sizeof(keyboard_report));
}
