#ifdef button_events_h
#define button_events_h

#include <stdint.h>
#include <stdbool.h>

#define max_buttons 8
#define max_button_events 16

typedef enum {
    button_pressed,
    button_released
} button_event_t;

typedef struct {
    uint8_t button_id;
    button_event_t type;
    uint32_t timestamp_ms;
} button_event_data_t;

void button_events_init(void);

bool button_events_push(
    uint8_t button_id,
    button_event_t type,
    uint32_t timestamp_ms
);

bool button_events_pop(button_event_data_t *event);