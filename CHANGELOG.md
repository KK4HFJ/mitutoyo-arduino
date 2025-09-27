# Changelog

All notable changes to this project will be documented in this file.

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

