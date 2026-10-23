/**
 * Outside air temperature (OAT) probe for XCSoar (HID branch).
 *
 * Hardware:
 *   - Seeed XIAO RP2040
 *   - Adafruit MAX31865 PT1000 amp (ADA3648)
 *   - 2-wire PT1000 RTD
 *
 * Vendor HID report every 10 s (not USB serial):
 *   report id 1
 *   int16 little-endian hundredths of a degC
 *   uint8 status (0 = ok, 1 = fault)
 *
 * USB poll interval 255 ms (host tap only). Sensor still read every 10 s.
 *
 * VID/PID 2E8A:0A01. Needs an XCSoar fork that reads this HID report.
 * See docs/hid-xcsoar.md.
 */

#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <SPI.h>
#include <USB.h>
#include <tusb.h>
#include <tusb-hid.h>
#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <cmath>
#include <cstdint>

#include "CoreMutex.h"

// --- Wiring (XIAO RP2040 <-> ADA3648) ---
//   3V3  -> Vin
//   GND  -> GND
//   D8   -> CLK   (SPI SCK)
//   D9   -> SDO   (SPI MISO)
//   D10  -> SDI   (SPI MOSI)
//   D7   -> CS

static constexpr int PIN_CS = D7;

static constexpr float RREF = 4300.0f;
static constexpr float RNOMINAL = 1000.0f;
static constexpr uint32_t EMIT_PERIOD_MS = 10000;

static constexpr uint8_t OAT_STATUS_OK = 0;
static constexpr uint8_t OAT_STATUS_FAULT = 1;

// Earle core patches HID_REPORT_ID at byte 6. Keep the same 6-byte prefix
// as Keyboard/Joystick (USAGE_PAGE + USAGE + COLLECTION).
// Usage page 0xFF is vendor-ish, not a mouse, so Android should not steal it.
static const uint8_t desc_hid_report_oat[] = {
    HID_USAGE_PAGE(0xFF),
    HID_USAGE(0x01),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(1)
    HID_USAGE(0x02),
    HID_LOGICAL_MIN_N(-32768, 2),
    HID_LOGICAL_MAX_N(32767, 2),
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
    HID_USAGE(0x03),
    HID_LOGICAL_MIN(0),
    HID_LOGICAL_MAX_N(255, 2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
    HID_COLLECTION_END};

// Host may ask this often; we only refresh the value every EMIT_PERIOD_MS.
extern "C" {
int usb_hid_poll_interval = 255;
}

struct __attribute__((packed)) OatReport {
  int16_t temp_centi;
  uint8_t status;
};

Adafruit_MAX31865 rtd(PIN_CS);
static uint8_t hid_local_id;
static OatReport last_report = {0, OAT_STATUS_FAULT};

static int16_t celsiusToCenti(float celsius) {
  const long centi = lround(static_cast<double>(celsius) * 100.0);
  if (centi > 32767) {
    return 32767;
  }
  if (centi < -32768) {
    return static_cast<int16_t>(-32768);
  }
  return static_cast<int16_t>(centi);
}

static void emitHid(int16_t temp_centi, uint8_t status) {
  last_report.temp_centi = temp_centi;
  last_report.status = status;

  CoreMutex m(&USB.mutex);
  tud_task();
  if (USB.HIDReady()) {
    tud_hid_n_report(0, USB.findHIDReportID(hid_local_id), &last_report,
                     sizeof(last_report));
  }
  tud_task();
}

static void ledsOff() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, HIGH);
  digitalWrite(PIN_LED_G, HIGH);
  digitalWrite(PIN_LED_B, HIGH);

  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, LOW);
}

static void sensorSleep() {
  rtd.enableBias(false);
  rtd.autoConvert(false);
}

void setup() {
  ledsOff();

  USB.disconnect();
  hid_local_id = USB.registerHIDDevice(desc_hid_report_oat, sizeof(desc_hid_report_oat),
                                       40, 0);
  USB.connect();

  SPI.begin();
  rtd.begin(MAX31865_2WIRE);
  sensorSleep();
}

void loop() {
  const uint32_t started_ms = millis();

  const uint16_t raw = rtd.readRTD();
  (void)raw;
  const uint8_t fault = rtd.readFault();
  if (fault) {
    rtd.clearFault();
    sensorSleep();
    emitHid(last_report.temp_centi, OAT_STATUS_FAULT);
  } else {
    const float celsius = rtd.temperature(RNOMINAL, RREF);
    sensorSleep();
    emitHid(celsiusToCenti(celsius), OAT_STATUS_OK);
  }

  const uint32_t used_ms = millis() - started_ms;
  if (used_ms < EMIT_PERIOD_MS) {
    delay(EMIT_PERIOD_MS - used_ms);
  }
}
