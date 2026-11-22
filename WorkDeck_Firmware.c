#include <stdio.h>

#include "pico/stdlib.h"

#include "button_scanner.h"
#include "button_events.h"
#include "usb_hid.h"
#include "keymaps.h"

int main(void)
{
    stdio_init_all(); // inits io shi if fails then just stops
    
    // init usb hid
    usb_hid_init();
    
    button_events_init();
    button_scanner_init();

    while (true) {
        
        // usb task gotta run fast
        usb_hid_task();
        
        // time
        uint32_t timestamp_ms = to_ms_since_boot(get_absolute_time());

        // scan buttons
        button_scanner_update(timestamp_ms);

        // Check button events
        button_event_data_t event;

        while (button_events_pop(&event)) {
            
            keymap_entry_t key = keymap_get(event.button_id);

            if (event.type == button_pressed) {
                // send the key
                if (key.keycode != 0) {
                    usb_hid_send_key_with_mod(key.keycode, key.modifier);
                }
                printf("btn %d pressed -> keycode %d\n", event.button_id, key.keycode);
            }
            else if (event.type == button_released) {
                // keys already released in send_key func
                printf("btn %d released\n", event.button_id);
            }
        }

        sleep_ms(1);
    }
}