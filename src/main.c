#include <stdio.h>

#include "pico/stdlib.h"

#include "buttons/button_scanner.h"
#include "buttons/button_events.h"
#include "comms/usb_comms.h"
#include "comms/protocol.h"

// heartbeat interval (120 seconds in ms)
#define HEARTBEAT_INTERVAL_MS 120000

// TODO: add led indicators maybe??
// TODO: sleep mode when idle

int main(void)
{
    stdio_init_all();
    
    // init protocol (generates session id)
    protocol_init();
    
    // init usb cdc
    usb_comms_init();
    
    button_events_init();
    button_scanner_init();
    
    uint32_t last_heartbeat = 0;

    while (true) {
        
        // usb task - must run fast
        usb_comms_task();
        
        // get current time
        uint32_t timestamp_ms = to_ms_since_boot(get_absolute_time());
        
        // send heartbeat periodically for time tracking
        if (timestamp_ms - last_heartbeat >= HEARTBEAT_INTERVAL_MS) {
            if (usb_comms_connected()) {
                usb_comms_send_heartbeat();
                last_heartbeat = timestamp_ms;
                // printf("heartbeat sent\n");  // debug
            }
        }

        // scan buttons
        button_scanner_update(timestamp_ms);

        // process button events
        button_event_data_t event;

        while (button_events_pop(&event)) {
            
            if (event.type == button_pressed) {
                // send button press signal to companion app
                usb_comms_send_button_signal(event.button_id, PACKET_TYPE_BUTTON_PRESS);
                printf("btn %d pressed -> signal sent\n", event.button_id);
            }
            else if (event.type == button_released) {
                // send button release signal
                usb_comms_send_button_signal(event.button_id, PACKET_TYPE_BUTTON_RELEASE);
                printf("btn %d released -> signal sent\n", event.button_id);
            }
        }

        sleep_ms(1);
    }
}