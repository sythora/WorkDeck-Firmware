#include "keymaps.h"

keymap_entry_t keymap_get(uint8_t button_id) {
    if (button_id < 1 || button_id > 8) {
        keymap_entry_t empty = {0, 0};
        return empty;
    }
    return default_keymap[button_id - 1];
}
