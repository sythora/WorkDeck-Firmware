#include "button_scanner.h"
#include "button_events.h"

#include "pico/stdlib.h"

#define debounce_ms 20

// GPIO pins for the columns
static const uint8_t column_pins[4] = {0, 1, 2, 3};

// GPIO pins for the rows
static const uint8_t row_pins[2] = {4, 5};

// Store the current state of each button
static bool raw_state[max_buttons];

// Store the confirmed state of each button
static bool stable_state[max_buttons];

// Store when the state last changed
static uint32_t raw_changed[max_buttons];


void button_scanner_init(void)
{
    // Init the columns as outputs
    for (int col = 0; col < 4; col++) {
        gpio_init(column_pins[col]);
        gpio_set_dir(column_pins[col], GPIO_OUT);
        gpio_put(column_pins[col], 0);
    }

    // Init the rows as inputs
    for (int row = 0; row < 2; row++) {
        gpio_init(row_pins[row]);
        gpio_set_dir(row_pins[row], GPIO_IN);
        gpio_pull_down(row_pins[row]);
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
    // Scan each column
    for (int col = 0; col < 4; col++) {

        // Set the column HIGH
        gpio_put(column_pins[col], 1);
        sleep_us(5);

        // Read each row
        for (int row = 0; row < 2; row++) {

            // Calculate the button index
            int button_index = (row * 4) + col;

            // Check if the button is pressed
            bool pressed = gpio_get(row_pins[row]);

            // Check if the state changed
            if (pressed != raw_state[button_index]) {
                raw_state[button_index] = pressed;
                raw_changed[button_index] = timestamp_ms;
            }

            // Check if the state has been stable for 20ms
            if ((timestamp_ms - raw_changed[button_index]) >= debounce_ms) {

                // Check if the confirmed state changed
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

        // Set the column LOW
        gpio_put(column_pins[col], 0);
    }
}