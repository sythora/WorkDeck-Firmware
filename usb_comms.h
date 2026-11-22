#ifndef usb_comms_h
#define usb_comms_h

#include <stdint.h>
#include <stdbool.h>
#include "protocol.h"

// init usb cdc (serial) communication
void usb_comms_init(void);

// must be called regularly in main loop
void usb_comms_task(void);

// check if connected to a host
bool usb_comms_connected(void);

// send a signal packet to companion app
bool usb_comms_send_packet(const signal_packet_t* packet);

// convenience: send button event signal
void usb_comms_send_button_signal(uint8_t button_id, packet_type_t type);

// send heartbeat for keepalive / time tracking
void usb_comms_send_heartbeat(void);

// read any incoming data from companion app
int usb_comms_read(uint8_t* buf, int max_len);

// LED control (not implemented yet)
// void usb_comms_set_led(uint8_t button_id, bool state);

#endif
