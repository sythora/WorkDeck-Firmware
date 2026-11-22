#include "usb_comms.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

// tinyusb cdc includes
#include "tusb.h"

// receive buffer for incoming commands from companion app
#define RX_BUF_SIZE 64
static uint8_t rx_buffer[RX_BUF_SIZE];
static int rx_len = 0;

void usb_comms_init(void) {
    // init tinyusb device stack
    tusb_init();
    rx_len = 0;
}

void usb_comms_task(void) {
    // tinyusb device task - must be called regularly
    tud_task();
    
    // check for incoming data
    if (tud_cdc_available()) {
        uint32_t count = tud_cdc_read(rx_buffer + rx_len, RX_BUF_SIZE - rx_len);
        rx_len += count;
        
        // TODO: parse incoming commands from companion app
        // for now just clear buffer when full
        if (rx_len >= RX_BUF_SIZE) {
            rx_len = 0;
        }
    }
}

bool usb_comms_connected(void) {
    // check if CDC is connected to a host
    return tud_cdc_connected();
}

bool usb_comms_send_packet(const signal_packet_t* packet) {
    if (!tud_cdc_connected()) {
        return false;
    }
    
    // check if we can write
    uint32_t avail = tud_cdc_write_available();
    if (avail < PACKET_SIZE) {
        return false;
    }
    
    // send the raw packet bytes
    uint32_t written = tud_cdc_write((const uint8_t*)packet, PACKET_SIZE);
    tud_cdc_write_flush();
    
    return written == PACKET_SIZE;
}

void usb_comms_send_button_signal(uint8_t button_id, packet_type_t type) {
    signal_packet_t packet;
    
    // create the packet
    protocol_create_button_packet(&packet, button_id, type);
    
    // encode/obfuscate it
    protocol_encode(&packet);
    
    // send it
    usb_comms_send_packet(&packet);
}

void usb_comms_send_heartbeat(void) {
    signal_packet_t packet;
    
    // create heartbeat packet (button_id = 0)
    protocol_create_button_packet(&packet, 0, PACKET_TYPE_HEARTBEAT);
    
    // encode and send
    protocol_encode(&packet);
    usb_comms_send_packet(&packet);
}

int usb_comms_read(uint8_t* buf, int max_len) {
    if (!tud_cdc_connected()) {
        return 0;
    }
    
    uint32_t avail = tud_cdc_available();
    if (avail == 0) {
        return 0;
    }
    
    uint32_t to_read = (avail > (uint32_t)max_len) ? (uint32_t)max_len : avail;
    uint32_t read = tud_cdc_read(buf, to_read);
    
    return (int)read;
}

// LED control stuff (wip)
// static bool led_states[8] = {0};
// 
// void usb_comms_set_led(uint8_t button_id, bool state) {
//     if (button_id < 1 || button_id > 8) return;
//     led_states[button_id - 1] = state;
//     // TODO: actually control the LED GPIO
// }
