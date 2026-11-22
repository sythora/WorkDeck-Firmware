#ifndef protocol_h
#define protocol_h

#include <stdint.h>
#include <stdbool.h>

// magic bytes for packet identification
#define PROTO_MAGIC_1 0x57  // 'W'
#define PROTO_MAGIC_2 0x44  // 'D'

// packet types
typedef enum {
    PACKET_TYPE_BUTTON_PRESS = 0x01,
    PACKET_TYPE_BUTTON_RELEASE = 0x02,
    PACKET_TYPE_HEARTBEAT = 0x03,
    PACKET_TYPE_PING = 0x04,
    PACKET_TYPE_PONG = 0x05,
    // PACKET_TYPE_MACRO_START = 0x10,  // not implemented yet
    // PACKET_TYPE_MACRO_STOP = 0x11,
} packet_type_t;

// button signal packet structure
// total 16 bytes for clean alignment
typedef struct __attribute__((packed)) {
    uint8_t magic1;         // 0x57 'W'
    uint8_t magic2;         // 0x44 'D'
    uint8_t packet_type;    // packet_type_t
    uint8_t button_id;      // 1-8
    uint32_t timestamp;     // ms since boot (obfuscated)
    uint32_t session_id;    // changes each boot
    uint16_t checksum;      // simple xor checksum
    uint8_t reserved[2];    // padding (using for noise now)
} signal_packet_t;

#define PACKET_SIZE sizeof(signal_packet_t)

// init protocol (generates session id etc)
void protocol_init(void);

// create a button signal packet
void protocol_create_button_packet(signal_packet_t* packet, uint8_t button_id, packet_type_t type);

// calculate checksum
uint16_t protocol_checksum(const signal_packet_t* packet);

// validate a packet
bool protocol_validate(const signal_packet_t* packet);

// encode packet for transmission
void protocol_encode(signal_packet_t* packet);

// decode packet
void protocol_decode(signal_packet_t* packet);

// get raw bytes for transmission
const uint8_t* protocol_get_bytes(const signal_packet_t* packet);

#endif
