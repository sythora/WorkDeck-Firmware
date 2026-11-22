#ifndef button_scanner_h
#define button_scanner_h

#include <stdint.h>
#include <stdbool.h>
void button_scanner_init(void);
void button_scanner_update(uint32_t timestamp_ms);

#endif