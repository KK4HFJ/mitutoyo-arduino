# Mitutoyo 52-bit Digimatic → Arduino

Workshop-ready reference for reading Mitutoyo SPC (Digimatic) 52-bit frames on an Arduino.

## Folder Layout
- **src/Mitutoyo_52bit_Reader/** - Arduino sketch
- **docs/** - guides and one-pagers
  - **diagrams/** - PNG diagrams used in the docs
- **examples/** - sample raw frames

## Quick Start
1. Wire DATA→D2, CLK→D3 (INT1), GND→GND. Drive REQ from D5 through a PN2222A with a 10k base resistor (Arduino HIGH pulls REQ low), as in the [original Instructable](https://www.instructables.com/Interfacing-a-Digital-Micrometer-to-a-Microcontrol/). Mitutoyo DATA/CK are open-collector, so the Arduino's internal pull-ups are all they need.
2. Open `src/Mitutoyo_52bit_Reader/Mitutoyo_52bit_Reader.ino` in the Arduino IDE and upload it to an UNO or Nano.
3. Open the Serial Monitor at 115200 to see the raw digits and the decoded value.

If you drive REQ directly through a level shifter instead of a transistor, set `REQ_ASSERT` to `LOW` in the sketch.

## Frame Format

Each reading is 13 digits (d1-d13) of 4 bits each, 52 bits total. Digits arrive in order d1 → d13, and each digit arrives LSB first.

| Digit | Meaning |
|---|---|
| d1-d4 | Sync, always `F` |
| d5 | Sign: `0` = +, `8` = − |
| d6-d11 | Measured value, 6 BCD digits, most significant first |
| d12 | Decimal point position, 0-5 (digits right of the point) |
| d13 | Unit: `0` = mm, `1` = inch |

Example: `F F F F 0 0 1 2 3 4 5 3 0` = +12.345 mm. More in `examples/sample_nibbles.txt`.

> **Note:** the PDFs in `docs/` and the frame diagrams predate v1.1.0. They describe a different nibble mapping (8 LSD-first digits, sign in nibble 8) and different pins (CLK D2, DATA D4, REQ D3). Where they disagree, this README and the sketch are correct.

## Docs
- `docs/Mitutoyo_52bit_Complete_Guide_v2.pdf` - story, protocol, diagrams, worked example, full code.
- `docs/Mitutoyo_Arduino_Pinout_OnePager.pdf` - wiring cheat sheet.

## Links
- [Original Instructable with wiring and components](https://www.instructables.com/Interfacing-a-Digital-Micrometer-to-a-Microcontrol/)
- [Interface board (OSH Park)](https://oshpark.com/shared_projects/EMwqQFNh)

## License
MIT © 2025 Steve Spence
