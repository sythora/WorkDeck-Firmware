#include "usb_hid.h"
#include "pico/stdlib.h"
#include "tusb.h"

// Simple hid keyboard implementation
// 6 key rollover like a normal keyboard

void usb_hid_init(void) {
    // init tinyusb device stack
    tusb_init();
}

void usb_hid_task(void) {
    // gotta call this in the main loop
    tud_task();
}

bool usb_hid_is_ready(void) {
    return tud_mounted() && tud_hid_ready();
}

void usb_hid_send_key(uint8_t keycode) {
    if (!usb_hid_is_ready()) return;
    
    // press
    uint8_t keys[6] = {keycode, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(0, 0, keys);
    
    // small delay
    sleep_ms(10);
    
    // release
    tud_hid_keyboard_report(0, 0, NULL);
}

void usb_hid_send_key_with_mod(uint8_t keycode, uint8_t modifier) {
    if (!usb_hid_is_ready()) return;
    
    // press with modifier
    uint8_t keys[6] = {keycode, 0, 0, 0, 0, 0};
    tud_hid_keyboard_report(0, modifier, keys);
    
    sleep_ms(10);
    
    // release
    tud_hid_keyboard_report(0, 0, NULL);
}

void usb_hid_release_all(void) {
    if (!usb_hid_is_ready()) return;
    tud_hid_keyboard_report(0, 0, NULL);
}

// tinyusb callbacks
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t bufsize) {
    // not used but needs to exist
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    // not used but needs to exist
}
