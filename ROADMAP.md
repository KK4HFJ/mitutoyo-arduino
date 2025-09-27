# Roadmap

Planned and potential improvements for the Mitutoyo 52-bit Digimatic → Arduino project.

## Near Term (v1.x)
- [ ] Add additional worked examples (negative numbers, inch mode, overrange / battery status).
- [ ] Expand documentation for common SPC connector pinouts (different Mitutoyo cables).
- [ ] Provide Fritzing / KiCad schematic for level shifting with optocouplers.
- [ ] Add Arduino Library Manager support (wrap `.ino` into a reusable library class).

## Mid Term (v2.x)
- [ ] Port to **ESP32** and **STM32** (hardware timers, multiple channels).
- [ ] Add **Raspberry Pi / Python host decoder** (via GPIO or USB interface).
- [ ] Wireless logging options (Bluetooth LE / WiFi).
- [ ] Add unit tests using Arduino CI frameworks.

## Long Term (v3.x)
- [ ] Support for **other caliper protocols** (Chinese BCD, binary formats).
- [ ] Integration with CNC / DRO software (e.g., LinuxCNC, GRBL plugins).
- [ ] Web-based visualization of live caliper data (browser dashboard).

## Stretch Goals
- [ ] Build a full **open-source DRO system** powered by Arduino/ESP32 + Mitutoyo/Igaging inputs.
- [ ] Offer PCB / kit designs for makerspaces and hobby machinists.

---

Contributions welcome! Open issues or submit PRs to help extend the project.
