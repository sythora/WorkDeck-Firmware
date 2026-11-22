# WorkDeck Firmware ♥

Firmware for a DIY streamdeck thing built on Raspberry Pi Pico with a 4x2 button matrix.

## What it does 

- 8 buttons in a 4x2 grid (more buttons = more problems, I advise you that if you want to expand on my pcb make your own bc you will have problems <3)
- Sends signals to a companion app over USB serial (CDC)
- Not keyboard keys, custom protocol so the app decides what each button does
- Press button, thing happens. Simple.

## Hardware 🔌

- Raspberry Pi Pico (the cheap  liek 6 one)
- my pcb (link to file, preferrably pre-assembled)

## Building 🛠️

Need the Pico SDK installed. Then:

```
mkdir build
cd build
cmake ..
ninja
```

Flash the .uf2 file to your Pico (hold BOOTSEL, plug in, drag file). If it doesn't work, try again. If it still doesn't work, google it, cuz wtf am I, a engineer?

## Protocol 📡

Sends 16-byte packets over USB serial:

| Byte | Field |
|------|-------|
| 0 | Magic 0x57 ('W') |
| 1 | Magic 0x44 ('D') |
| 2 | Packet type |
| 3 | Button ID (1-8) |
| 4-7 | Timestamp |
| 8-11 | Session ID |
| 12-13 | Checksum |
| 14-15 | Reserved |

Packet types:
- 0x01 = button press
- 0x02 = button release  
- 0x03 = heartbeat (i'm alive!)

Payload is XOR encoded after the magic bytes. The companion app needs to decode it. Good luck, you have my code .

## Files 📁

- `WorkDeck_Firmware.c` - main loop, the big boss
- `button_scanner.c/h` - matrix scanning + debounce magic
- `button_events.c/h` - event queue
- `protocol.c/h` - packet creation + encoding
- `usb_comms.c/h` - USB CDC stuff
- `usb_descriptors.c` - USB descriptors (boring but necessary)

## TODO 📝

- [ ] LED control per button (one day...)
- [ ] Macro recording maybe
- [ ] Better protocol docs
- [ ] Actually write the companion app
- [ ] World domination

## Contributing

If you want to contribute, go ahead. I'm not your mom.

## License

idk do whatever ¯\_(ツ)_/¯
