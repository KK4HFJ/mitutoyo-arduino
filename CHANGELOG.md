# Changelog

All notable changes to this project will be documented in this file.

## [1.1.0] - 2026-11-20
### Fixed
- Decoder now follows the Mitutoyo Digimatic frame: d1-d4 sync (`F`), d5 sign, d6-d11 value (MSD first), d12 decimal point, d13 unit. The 1.0.0 mapping (N0-N7 digits LSD first, sign in N8) did not decode real instrument frames.
- Frames are validated (sync, sign, BCD, decimal, unit) before a value is printed.
- A zero fractional part printed one extra `0` (e.g. `12.0000` for 3 decimals).
- Removed a stray `\` on the first line of the sketch.

### Changed
- Sketch moved to `src/Mitutoyo_52bit_Reader/` so the Arduino IDE and `arduino-cli` can open and compile it.
- REQ assert level is configurable (`REQ_ASSERT`) for level-shifter or transistor drive.
- `examples/sample_nibbles.txt` rewritten in the real frame format.

### Added
- GitHub Actions compile check for UNO and Nano, and `.gitignore` (both were listed for 1.0.0 but never committed).

### Removed
- Stray `mitutoyo-arduino-repo-with-ci (1).zip` committed at the repo root.

## [1.0.0] - 2025-09-27
### Added
- Initial public release of the Mitutoyo 52-bit Digimatic → Arduino reference.
- Complete historical documentation:
  - `docs/Mitutoyo_52bit_History_and_Code.pdf`
  - `docs/Mitutoyo_52bit_History_and_Code_with_Diagrams.pdf`
  - `docs/Mitutoyo_52bit_Complete_Guide.pdf`
  - `docs/Mitutoyo_52bit_Complete_Guide_v2.pdf` (with worked example decode)
- Canonical Arduino sketch:
  - `src/Mitutoyo_52bit_Reader.ino`
- Quick reference one-pager:
  - `docs/Mitutoyo_Arduino_Pinout_OnePager.pdf`
- Diagrams in `docs/diagrams/`
- Example raw nibble dumps in `examples/sample_nibbles.txt`
- Project-level `README.md` with quick start guide.
- `LICENSE` (MIT) and `.gitignore` for repo hygiene.

### Notes
- This release formalizes the solution first published by Steve Spence (sspence) on Instructables and the Arduino Forum (2010–2011), combined with Mitutoyo’s official protocol details (Manual 4330).
- Future releases may add more example decoders, support for other microcontrollers (ESP32, STM32), and Python/RPi host code.

