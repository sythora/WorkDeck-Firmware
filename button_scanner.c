#include "button_scanner.h"
#include "button_events.h"

#include "pico/stdlib.h"

#define debounce_ms 20

// GPIO pins (refer to switchmatrixlayout.md)
//
// Column 0 = GP0
// Column 1 = GP1
// Column 2 = GP2
// Column 3 = GP3

static const uint8_t column_pins[4] = {0, 1, 2, 3};

// GPIO pins for the 2 rows
//
// Row 0 = GP4
// Row 1 = GP5

static const uint8_t row_pins[2] = {4, 5};

// Store the most recent state of each button
static bool raw_state[max_buttons];

// Store the confirmed state of each button
static bool stable_state[max_buttons];

// Store when the state last changed for each button
static uint32_t raw_changed[max_buttons];


void button_scanner_init(void)
{
    // Init the pins as inputs, the pull-up resistors
    // keep the columns HIGH when nothing is pressed
    for (int col = 0; col < 4; col++) {
        gpio_init(column_pins[col]);
        gpio_set_dir(column_pins[col], GPIO_IN);
        gpio_pull_up(column_pins[col]);
    }

    // Init the GPIO pins as outputs
    for (int row = 0; row < 2; row++) {
        gpio_init(row_pins[row]);
        gpio_set_dir(row_pins[row], GPIO_OUT);
        gpio_put(row_pins[row], 1);
    }

    // Set the initial state of each button
    for (int i = 0; i < max_buttons; i++) {
        raw_state[i] = false;
        stable_state[i] = false;
        raw_changed[i] = 0;
    }
}


void button_scanner_update(uint32_t timestamp_ms)
{
    // Scan each row
    for (int row = 0; row < 2; row++) {

        // Set the row to LOW
        gpio_put(row_pins[row], 0);

        // Read each column
        for (int col = 0; col < 4; col++) {

            // Calculate the button index
            int button_index = (row * 4) + col;

            // Read the column to check if the button is pressed
            bool pressed = !gpio_get(column_pins[col]);

            // Check if the button state changed
            if (pressed != raw_state[button_index]) {
                raw_state[button_index] = pressed;
                raw_changed[button_index] = timestamp_ms;
            }

            // Check if the state has been stable for 20ms
            if ((timestamp_ms - raw_changed[button_index]) >= debounce_ms) {

                // Check if the confirmed state needs to change
                if (stable_state[button_index] != raw_state[button_index]) {

                    stable_state[button_index] = raw_state[button_index];

                    // Send the button event
                    button_events_push(
                        button_index + 1,
                        stable_state[button_index]
                            ? button_pressed
                            : button_released,
                        timestamp_ms
                    );
                }
            }
        }

        // Set the row back to HIGH
        gpio_put(row_pins[row], 1);
    }
}