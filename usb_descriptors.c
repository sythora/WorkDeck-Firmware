#include "tusb.h"
#include "pico/unique_id.h"

// device descriptor
tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,      // each interface defines class
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    
    // vid/pid - using pico defaults
    .idVendor = 0x2E8A,
    .idProduct = 0x000A,       // CDC serial
    .bcdDevice = 0x0100,
    
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

// CDC endpoints
#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x82

// configuration descriptor - CDC only
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)

uint8_t const desc_configuration[] = {
    // config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, 2, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_SELF_POWERED, 100),
    
    // CDC: interface number, string index, EP notification address and size, EP data address (out, in) and size
    TUD_CDC_DESCRIPTOR(0, 0, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64)
};

// tinyusb callbacks
uint8_t const* tud_descriptor_device_cb(void) {
    return (uint8_t const*)&desc_device;
}

uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return desc_configuration;
}

// no HID - we're CDC only

// string descriptors
static char serial_str[16];

uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    
    static uint16_t str_buf[32];
    uint8_t chr_count;
    
    switch(index) {
        case 0: // language
            str_buf[0] = (TUSB_DESC_STRING << 8) | 2;
            str_buf[1] = 0x0409; // english
            return str_buf;
            
        case 1: // manufacturer
            chr_count = strlen("WorkDeck");
            str_buf[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);
            for(uint8_t i = 0; i < chr_count; i++) {
                str_buf[1+i] = "WorkDeck"[i];
            }
            return str_buf;
            
        case 2: // product
            chr_count = strlen("WorkDeck Keyboard");
            str_buf[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);
            for(uint8_t i = 0; i < chr_count; i++) {
                str_buf[1+i] = "WorkDeck Keyboard"[i];
            }
            return str_buf;
            
        case 3: // serial - use pico unique id
            pico_get_unique_board_id_string(serial_str, sizeof(serial_str));
            chr_count = strlen(serial_str);
            str_buf[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);
            for(uint8_t i = 0; i < chr_count; i++) {
                str_buf[1+i] = serial_str[i];
            }
            return str_buf;
            
        default:
            return NULL;
    }
}
