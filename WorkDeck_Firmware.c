#include <stdio.h>

#include "pico/stdlib.h"

#include "button_scanner.h"
#include "button_events.h"

int main(void)
{
    stdio_init_all(); // inits io shi if fails then just stops

    button_events_init();
    button_scanner_init();

    while (true) {

        // time
        uint32_t timestamp_ms = to_ms_since_boot(get_absolute_time());

        // scan buttons
        button_scanner_update(timestamp_ms);

        // Check button events
        button_event_data_t event;

        while (button_events_pop(&event)) {

            // Print event for tests
            if (event.type == button_pressed) {
                printf("Button %d pressed\n", event.button_id);
            }
            else if (event.type == button_released) {
                printf("Button %d released\n", event.button_id);
            }
        }

        sleep_ms(1);
    }
}