#ifndef tusb_config_h
#define tusb_config_h

#ifdef __cplusplus
extern "C" {
#endif

// RHPort number is 0 for Pico
#define BOARD_TUD_RHPORT 0

// max device
#define CFG_TUD_MAX_DEVICE 1

// buffer sizes
#define CFG_TUD_ENDPOINT0_SIZE 64

// CDC (serial)
#define CFG_TUD_CDC 1
#define CFG_TUD_CDC_RX_BUFSIZE 64
#define CFG_TUD_CDC_TX_BUFSIZE 64

#ifdef __cplusplus
}
#endif

#endif
