/**
 * Outside air temperature (OAT) probe for XCSoar.
 *
 * Hardware:
 *   - Seeed XIAO RP2040
 *   - Adafruit MAX31865 PT1000 amp (ADA3648)
 *   - 2-wire PT1000 RTD
 *
 * Emits OpenVario NMEA over USB CDC every 10 s:
 *   $POV,T,<celsius>*CS\r\n
 *
 * Between samples: RTD bias off, LEDs off, MCU sleeps (USB stays up for Stefly).
 *
 * XCSoar:
 *   Devices -> USB serial -> driver "OpenVario", baud 115200
 *   Show InfoBox "Outside air temperature"
 */

#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <SPI.h>
#include <cstdio>

// --- Wiring (XIAO RP2040 <-> ADA3648) ---
//   3V3  -> Vin
//   GND  -> GND
//   D8   -> CLK   (SPI SCK)
//   D9   -> SDO   (SPI MISO)
//   D10  -> SDI   (SPI MOSI)
//   D7   -> CS
//
// PT1000 2-wire on ADA3648:
//   One sense wire in either left terminal, the other in either right terminal.
//   Solder-close both triangular "2 Wire" jumpers under the terminal block
//   (or short the two left pads together and the two right pads together).

static constexpr int PIN_CS = D7;

// ADA3648 has a 4300 Ohm reference resistor for PT1000.
static constexpr float RREF = 4300.0f;
static constexpr float RNOMINAL = 1000.0f;

// OAT changes slowly in flight; 10 s is enough for XCSoar.
static constexpr uint32_t EMIT_PERIOD_MS = 10000;

Adafruit_MAX31865 rtd(PIN_CS);

static uint8_t nmeaChecksum(const char *body) {
  uint8_t cs = 0;
  while (*body) {
    cs ^= static_cast<uint8_t>(*body++);
  }
  return cs;
}

static void emitOat(float celsius) {
  // dtostrf: newlib-nano printf has no %f unless extra float support is linked.
  char temp[12];
  dtostrf(static_cast<double>(celsius), 0, 2, temp);

  char body[32];
  snprintf(body, sizeof(body), "POV,T,%s", temp);

  char line[40];
  snprintf(line, sizeof(line), "$%s*%02X\r\n", body, nmeaChecksum(body));
  Serial.print(line);
}

static void reportFault(uint8_t fault) {
  if (fault & MAX31865_FAULT_HIGHTHRESH) {
    Serial.println(F("# FAULT: RTD high threshold (open?)"));
  }
  if (fault & MAX31865_FAULT_LOWTHRESH) {
    Serial.println(F("# FAULT: RTD low threshold (short?)"));
  }
  if (fault & MAX31865_FAULT_REFINLOW) {
    Serial.println(F("# FAULT: REFIN- > 0.85 x Bias"));
  }
  if (fault & MAX31865_FAULT_REFINHIGH) {
    Serial.println(F("# FAULT: REFIN- < 0.85 x Bias - FORCE- open"));
  }
  if (fault & MAX31865_FAULT_RTDINLOW) {
    Serial.println(F("# FAULT: RTDIN- < 0.85 x Bias - FORCE- open"));
  }
  if (fault & MAX31865_FAULT_OVUV) {
    Serial.println(F("# FAULT: Under/Over voltage"));
  }
}

static void ledsOff() {
  // XIAO RGB are active-low; drive high to extinguish.
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, HIGH);
  digitalWrite(PIN_LED_G, HIGH);
  digitalWrite(PIN_LED_B, HIGH);

  // Cut power to the onboard NeoPixel.
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, LOW);
}

static void sensorSleep() {
  rtd.enableBias(false);
  rtd.autoConvert(false);
}

void setup() {
  ledsOff();

  Serial.begin(115200);
  delay(500);

  SPI.begin();
  if (!rtd.begin(MAX31865_2WIRE)) {
    Serial.println(F("# MAX31865 begin() failed"));
  }
  sensorSleep();

  Serial.println(F("# OAT probe ready - $POV,T for XCSoar OpenVario"));
}

void loop() {
  const uint32_t started_ms = millis();

  // readRTD() briefly enables bias, then turns it off again.
  const uint16_t raw = rtd.readRTD();
  const uint8_t fault = rtd.readFault();
  if (fault) {
    reportFault(fault);
    rtd.clearFault();
    sensorSleep();
  } else {
    const float ratio = raw / 32768.0f;
    const float resistance = RREF * ratio;
    const float celsius = rtd.temperature(RNOMINAL, RREF);

    Serial.print(F("# R="));
    Serial.print(resistance, 2);
    Serial.print(F(" Ohm  T="));
    Serial.print(celsius, 2);
    Serial.println(F(" C"));

    emitOat(celsius);
    sensorSleep();
  }

  // Sleep the rest of the period (idle CPU; USB CDC stays connected).
  const uint32_t used_ms = millis() - started_ms;
  if (used_ms < EMIT_PERIOD_MS) {
    delay(EMIT_PERIOD_MS - used_ms);
  }
}
