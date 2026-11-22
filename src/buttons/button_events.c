#ifndef button_events_c
#define button_events_c

#include "button_events.h"
#include <stddef.h>

static button_event_data_t queue[max_button_events];

static uint8_t head = 0;
static uint8_t tail = 0;
static uint8_t count = 0;

void button_events_init(void) {
    head = 0;
    tail = 0;
    count = 0;
}

bool button_events_push(
    uint8_t button_id,
    button_event_t type,
    uint32_t timestamp_ms
) {
    if (button_id < 1 || button_id > max_buttons) {
        return false;
    }

    if (count >= max_button_events) {
        return false;
    }

    queue[tail].button_id = button_id;
    queue[tail].type = type;
    queue[tail].timestamp_ms = timestamp_ms;

    tail = (tail + 1) % max_button_events;
    count++;

    return true;
}

bool button_events_pop(button_event_data_t *event) {
    if (count == 0 || event == NULL) {
        return false;
    }

    *event = queue[head];

    head = (head + 1) % max_button_events;
    count--;

    return true;
}
#endif