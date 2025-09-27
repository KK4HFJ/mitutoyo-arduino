\
/*
  Mitutoyo / Digimatic 52-bit Reader (UNO/Nano)
  ------------------------------------------------
  • Captures the 52-bit (13-nibble) LSB-first frame on CLK falling edges.
  • Prints raw nibbles and a decoded, human-readable value.
  • Supports optional REQ (active low) to trigger a reading.
  • IMPORTANT: Use proper level shifting or optocouplers for SPC lines.
    Do NOT connect caliper lines directly to 5V Arduino pins.

  Wiring (Arduino side, via level shifting):
    CLK  -> D2  (INT0)
    DATA -> D4
    REQ  -> D3  (optional output, active LOW)
    GND  -> GND

  Frame layout (common/hobbyist canonical mapping):
    N0..N7  : 8 BCD digits, least significant digit first (N0 = 10^0)
    N8      : sign (bit3 set => negative)
    N9      : decimal places (0..7 typical)
    N10     : units (0 = mm, 1 = inch)
    N11..12 : status flags (overrange, battery, etc. model-dependent)
*/

#define CLK_PIN   2   // INT0
#define DATA_PIN  4
#define REQ_PIN   3

// If your device free-runs (no request needed), set this false.
const bool USE_REQ = true;

// Timing (tweak as needed)
const unsigned long REQ_LOW_MS        = 120; // hold REQ low to trigger a burst
const unsigned long FRAME_TIMEOUT_MS  = 200; // max wait for full 52-bit frame

volatile uint64_t frameBits = 0;
volatile uint8_t  bitCount  = 0;
volatile bool     frameReady = false;

void isrClockFalling() {
  // Read DATA on falling edge per Mitutoyo timing
  uint8_t bit = digitalRead(DATA_PIN) ? 1 : 0;

  // Store LSB-first
  if (bitCount < 64) {
    frameBits |= (uint64_t)bit << bitCount;
    bitCount++;
    if (bitCount == 52) {
      frameReady = true;
      detachInterrupt(digitalPinToInterrupt(CLK_PIN)); // stop capture until processed
    }
  }
}

void startCapture() {
  noInterrupts();
  frameBits = 0;
  bitCount  = 0;
  frameReady = false;
  interrupts();

  attachInterrupt(digitalPinToInterrupt(CLK_PIN), isrClockFalling, FALLING);
}

void sendRequestPulseIfEnabled() {
  if (!USE_REQ) return;
  digitalWrite(REQ_PIN, LOW);
  delay(REQ_LOW_MS);
  digitalWrite(REQ_PIN, HIGH);
}

void printNibbles(uint64_t bits) {
  Serial.print(F("Raw nibbles (LSB->MSB): "));
  for (int i = 0; i < 13; i++) {
    uint8_t nib = (bits >> (i * 4)) & 0xF;
    Serial.print(nib, HEX);
    if (i < 12) Serial.print(' ');
  }
  Serial.println();
}

void printDecoded(uint64_t bits) {
  uint8_t nib[13];
  for (int i = 0; i < 13; i++) nib[i] = (bits >> (i * 4)) & 0xF;

  // Assemble BCD digits (N0 LSB first)
  long value = 0;
  long place = 1;
  bool bcdOk = true;
  for (int i = 0; i < 8; i++) {
    uint8_t d = nib[i];
    if (d > 9) bcdOk = false;
    value += (d % 10) * place;
    place *= 10;
  }

  bool negative = (nib[8] & 0x8) != 0;
  int  dp       = nib[9] & 0x7;
  bool isInch   = (nib[10] & 0x1) != 0;
  uint8_t s11   = nib[11], s12 = nib[12];

  long scale = 1;
  for (int i = 0; i < dp; i++) scale *= 10;
  long ip = (scale > 0) ? value / scale : value;
  long fp = (scale > 0) ? value % scale : 0;

  if (!bcdOk) {
    Serial.println(F("Note: Non-BCD digit(s) detected; verify nibble mapping for your instrument."));
  }

  Serial.print(F("Value: "));
  if (negative) Serial.print('-');
  Serial.print(ip);
  if (dp > 0) {
    Serial.print('.');
    // zero-pad fractional to dp width
    long pad = scale / 10;
    while (pad > 0 && fp < pad) { Serial.print('0'); pad /= 10; }
    Serial.print(fp);
  }
  Serial.println(isInch ? F(" in") : F(" mm"));

  Serial.print(F("Status nibbles (N11 N12): 0x"));
  Serial.print(s11, HEX);
  Serial.print(' ');
  Serial.println(s12, HEX);
}

bool captureOneFrame(uint64_t &outBits) {
  startCapture();
  sendRequestPulseIfEnabled();

  unsigned long t0 = millis();
  while (!frameReady && (millis() - t0) < FRAME_TIMEOUT_MS) {
    // wait for ISR to collect 52 bits
  }

  detachInterrupt(digitalPinToInterrupt(CLK_PIN));

  if (!frameReady) return false;

  noInterrupts();
  outBits = frameBits;
  interrupts();
  return true;
}

void setup() {
  Serial.begin(115200);

  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, INPUT_PULLUP);

  if (USE_REQ) {
    pinMode(REQ_PIN, OUTPUT);
    digitalWrite(REQ_PIN, HIGH); // idle high
  }

  Serial.println(F("Mitutoyo 52-bit reader ready."));
  Serial.println(F("Ensure proper isolation/level shifting for SPC lines."));
}

void loop() {
  uint64_t bits;
  if (captureOneFrame(bits)) {
    printNibbles(bits);
    printDecoded(bits);
    Serial.println();
  } else {
    // no frame captured within timeout; adjust timings or wiring if needed
    delay(60);
  }

  delay(60);
}
