/*
  Mitutoyo / Digimatic 52-bit Reader (UNO/Nano)
  ---------------------------------------------
  - Captures one 52-bit Digimatic frame on CLK falling edges.
  - Prints the 13 raw digits and the decoded, human-readable value.
  - Optionally drives REQ to ask the instrument for a reading.
  - IMPORTANT: use level shifting or optocouplers on the SPC lines.
    Do NOT connect caliper lines directly to 5 V Arduino pins.

  Wiring (Arduino side, via level shifting):
    CLK  -> D2 (INT0)
    DATA -> D4
    REQ  -> D3 (optional output)
    GND  -> GND

  Frame layout (Mitutoyo Digimatic: 13 digits d1..d13, 4 bits each,
  digits sent in order d1 -> d13, each digit LSB first):
    d1..d4  : always 0xF (sync)
    d5      : sign (0 = +, 8 = -)
    d6..d11 : measured value, 6 BCD digits, most significant first
    d12     : decimal point position (0..5 = digits right of the point)
    d13     : unit (0 = mm, 1 = inch)
*/

// ---- Configuration ---------------------------------------------------------

constexpr uint8_t CLK_PIN  = 2;  // must be an external-interrupt pin
constexpr uint8_t DATA_PIN = 4;
constexpr uint8_t REQ_PIN  = 3;

// Set false if your instrument free-runs (no request needed).
constexpr bool USE_REQ = true;

// Pin level that asserts REQ. The instrument's REQ is active low: use LOW
// when driving it through a level shifter, HIGH when driving it through an
// NPN transistor that pulls the line low.
constexpr uint8_t REQ_ASSERT = LOW;

constexpr unsigned long SERIAL_BAUD      = 115200;
constexpr unsigned long REQ_HOLD_MS      = 120;  // how long REQ is held asserted
constexpr unsigned long FRAME_TIMEOUT_MS = 200;  // max wait for a full frame
constexpr unsigned long LOOP_PAUSE_MS    = 60;   // pause between readings

// ---- Frame format ----------------------------------------------------------

constexpr uint8_t FRAME_BITS   = 52;
constexpr uint8_t FRAME_DIGITS = 13;

constexpr uint8_t SYNC_DIGITS  = 4;   // d1..d4
constexpr uint8_t SIGN_DIGIT   = 4;   // d5   (0-based index)
constexpr uint8_t VALUE_FIRST  = 5;   // d6
constexpr uint8_t VALUE_DIGITS = 6;   // d6..d11
constexpr uint8_t POINT_DIGIT  = 11;  // d12
constexpr uint8_t UNIT_DIGIT   = 12;  // d13

constexpr uint8_t SYNC_VALUE   = 0xF;
constexpr uint8_t SIGN_MINUS   = 8;
constexpr uint8_t MAX_DECIMALS = 5;

struct Reading {
  bool    valid;     // sync, sign, BCD, decimal and unit digits all checked out
  bool    negative;
  long    value;     // 6-digit integer, decimal point not yet applied
  uint8_t decimals;  // digits right of the decimal point
  bool    inch;
};

// ---- Capture (ISR) ---------------------------------------------------------

volatile uint64_t frameBits  = 0;
volatile uint8_t  bitCount   = 0;
volatile bool     frameReady = false;

// DATA is sampled on each CLK falling edge and stored LSB first, so digit i
// ends up in bits 4i..4i+3 with its own bit order intact.
void isrClockFalling() {
  if (bitCount >= FRAME_BITS) return;

  if (digitalRead(DATA_PIN)) {
    frameBits |= (uint64_t)1 << bitCount;
  }
  if (++bitCount == FRAME_BITS) {
    frameReady = true;
    detachInterrupt(digitalPinToInterrupt(CLK_PIN));  // hold until processed
  }
}

void startCapture() {
  noInterrupts();
  frameBits  = 0;
  bitCount   = 0;
  frameReady = false;
  interrupts();

  attachInterrupt(digitalPinToInterrupt(CLK_PIN), isrClockFalling, FALLING);
}

void sendRequestPulse() {
  if (!USE_REQ) return;
  digitalWrite(REQ_PIN, REQ_ASSERT);
  delay(REQ_HOLD_MS);
  digitalWrite(REQ_PIN, !REQ_ASSERT);
}

// Returns true and fills outBits if a full frame arrived before the timeout.
bool captureOneFrame(uint64_t &outBits) {
  startCapture();
  sendRequestPulse();

  const unsigned long t0 = millis();
  while (!frameReady && (millis() - t0) < FRAME_TIMEOUT_MS) {
    // ISR collects the bits
  }
  detachInterrupt(digitalPinToInterrupt(CLK_PIN));

  if (!frameReady) return false;

  noInterrupts();
  outBits = frameBits;
  interrupts();
  return true;
}

// ---- Decode ----------------------------------------------------------------

uint8_t digitAt(uint64_t bits, uint8_t index) {
  return (bits >> (index * 4)) & 0xF;
}

Reading decodeFrame(uint64_t bits) {
  Reading r = {};
  bool ok = true;

  for (uint8_t i = 0; i < SYNC_DIGITS; i++) {
    if (digitAt(bits, i) != SYNC_VALUE) ok = false;
  }

  const uint8_t sign = digitAt(bits, SIGN_DIGIT);
  if (sign != 0 && sign != SIGN_MINUS) ok = false;

  for (uint8_t i = 0; i < VALUE_DIGITS; i++) {
    const uint8_t d = digitAt(bits, VALUE_FIRST + i);
    if (d > 9) ok = false;
    r.value = r.value * 10 + d;
  }

  r.decimals = digitAt(bits, POINT_DIGIT);
  if (r.decimals > MAX_DECIMALS) ok = false;

  const uint8_t unit = digitAt(bits, UNIT_DIGIT);
  if (unit > 1) ok = false;

  r.negative = (sign == SIGN_MINUS);
  r.inch     = (unit == 1);
  r.valid    = ok;
  return r;
}

// ---- Output ----------------------------------------------------------------

void printRawDigits(uint64_t bits) {
  Serial.print(F("Raw digits d1..d13: "));
  for (uint8_t i = 0; i < FRAME_DIGITS; i++) {
    Serial.print(digitAt(bits, i), HEX);
    if (i < FRAME_DIGITS - 1) Serial.print(' ');
  }
  Serial.println();
}

void printReading(const Reading &r) {
  if (!r.valid) {
    Serial.println(F("Frame failed validation (sync/sign/BCD/decimal/unit). "
                     "Check wiring, clock edge and level shifting."));
    return;
  }

  long scale = 1;
  for (uint8_t i = 0; i < r.decimals; i++) scale *= 10;

  Serial.print(F("Value: "));
  if (r.negative) Serial.print('-');
  Serial.print(r.value / scale);

  if (r.decimals > 0) {
    const long frac = r.value % scale;
    Serial.print('.');
    for (long pad = scale / 10; pad > 1 && frac < pad; pad /= 10) {
      Serial.print('0');  // leading zeros of the fractional part
    }
    Serial.print(frac);
  }
  Serial.println(r.inch ? F(" in") : F(" mm"));
}

// ---- Arduino entry points --------------------------------------------------

void setup() {
  Serial.begin(SERIAL_BAUD);

  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, INPUT_PULLUP);

  if (USE_REQ) {
    pinMode(REQ_PIN, OUTPUT);
    digitalWrite(REQ_PIN, !REQ_ASSERT);  // idle = not requesting
  }

  Serial.println(F("Mitutoyo 52-bit reader ready."));
  Serial.println(F("Ensure proper isolation/level shifting for SPC lines."));
}

void loop() {
  uint64_t bits;
  if (captureOneFrame(bits)) {
    printRawDigits(bits);
    printReading(decodeFrame(bits));
    Serial.println();
  }
  // On timeout nothing is printed; if that persists, check timing or wiring.

  delay(LOOP_PAUSE_MS);
}
