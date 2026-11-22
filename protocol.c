#include "protocol.h"
#include "pico/stdlib.h"
#include <string.h>

// session id - random per boot
static uint32_t session_id = 0;

// obfuscation key (changes per session)
static uint8_t obf_key = 0;

// weird hack for entropy
static uint32_t xorshift_state = 1;

static uint32_t xorshift32(void) {
    uint32_t x = xorshift_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    xorshift_state = x;
    return x;
}

void protocol_init(void) {
    // generate pseudo-random session id from hardware
    // use timer as entropy source
    session_id = to_ms_since_boot(get_absolute_time());
    session_id ^= (session_id << 16);
    xorshift_state = session_id;  // seed the rng
    
    // derive obfuscation key
    obf_key = (uint8_t)(session_id & 0xFF);
}

uint16_t protocol_checksum(const signal_packet_t* packet) {
    uint16_t sum = 0;
    const uint8_t* data = (const uint8_t*)packet;
    
    // sum all bytes except checksum field
    for (int i = 0; i < 6; i++) {
        sum += data[i];
    }
    // skip checksum bytes (indices 6-7)
    for (int i = 8; i < PACKET_SIZE; i++) {
        sum += data[i];
    }
    
    return sum ^ 0xDEAD;  // xor with constant
}

void protocol_create_button_packet(signal_packet_t* packet, uint8_t button_id, packet_type_t type) {
    memset(packet, 0, PACKET_SIZE);
    
    packet->magic1 = PROTO_MAGIC_1;
    packet->magic2 = PROTO_MAGIC_2;
    packet->packet_type = (uint8_t)type;
    packet->button_id = button_id;
    
    // timestamp with session-based offset for obfuscation
    uint32_t raw_time = to_ms_since_boot(get_absolute_time());
    packet->timestamp = raw_time ^ session_id;
    
    packet->session_id = session_id;
    
    // add some noise to reserved bytes (why not)
    packet->reserved[0] = (uint8_t)(xorshift32() & 0xFF);
    packet->reserved[1] = (uint8_t)(xorshift32() & 0xFF);
    
    // calculate checksum
    packet->checksum = protocol_checksum(packet);
}

void protocol_encode(signal_packet_t* packet) {
    // simple xor obfuscation on the payload portion
    // keep magic bytes clear for framing
    uint8_t* data = (uint8_t*)packet;
    
    // TODO: maybe use a better encoding? this is pretty weak
    for (int i = 2; i < PACKET_SIZE; i++) {
        data[i] ^= obf_key;
    }
}

void protocol_decode(signal_packet_t* packet) {
    // reverse the obfuscation
    uint8_t* data = (uint8_t*)packet;
    
    for (int i = 2; i < PACKET_SIZE; i++) {
        data[i] ^= obf_key;
    }
}

bool protocol_validate(const signal_packet_t* packet) {
    // check magic bytes
    if (packet->magic1 != PROTO_MAGIC_1 || packet->magic2 != PROTO_MAGIC_2) {
        return false;
    }
    
    // verify checksum
    uint16_t calc_checksum = protocol_checksum(packet);
    if (packet->checksum != calc_checksum) {
        return false;
    }
    
    return true;
}

const uint8_t* protocol_get_bytes(const signal_packet_t* packet) {
    return (const uint8_t*)packet;
}
