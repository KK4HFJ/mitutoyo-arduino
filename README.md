# Mitutoyo 52‑bit Digimatic → Arduino

Canonical, workshop‑ready reference for reading Mitutoyo SPC (Digimatic) 52‑bit frames on Arduino.

## Folder Layout
- **src/** — Arduino sketches
- **docs/** — Guides and one‑pagers
  - **diagrams/** — PNG diagrams used in the docs
- **examples/** — Sample raw frames/dumps

## Quick Start
1. Wire SPC via level shifting (CLK→D2 INT0, DATA→D4, REQ→D3 optional, GND→GND).
2. Open `src/Mitutoyo_52bit_Reader.ino` in Arduino IDE and upload to UNO/Nano.
3. Serial Monitor @ 115200: view raw nibbles and decoded value.

## Docs
- `docs/Mitutoyo_52bit_Complete_Guide_v2.pdf` — story, protocol, diagrams, worked example, full code.
- `docs/Mitutoyo_Arduino_Pinout_OnePager.pdf` — wiring cheat‑sheet.


## Links
- `https://www.instructables.com/Interfacing-a-Digital-Micrometer-to-a-Microcontrol/` - Original instructable with wiring and components.

## License
MIT © 2025 Steve Spence
