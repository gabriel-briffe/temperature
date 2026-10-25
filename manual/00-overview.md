# 00 - Overview

## What this builds

A USB OAT sensor for XCSoar: PT1000 probe -> ADA3648 -> XIAO RP2040 -> USB -> flight computer.

![Parts overview](images/parts-gotronic.jpg)

| Role | Part |
|------|------|
| Brain + USB | Seeed XIAO RP2040 |
| RTD amp | Adafruit ADA3648 (MAX31865, PT1000) |
| Sensor | 2-wire PT1000 probe |

## Two options

Same hardware. Different firmware, different XCSoar side.

### Option 1 - serial (stock XCSoar)

Firmware: `firmware/serial/`. The XIAO is a USB serial device, `2E8A:000A`. It emits OpenVario OAT at 115200:

```text
$POV,T,<temperature_celsius>*CS
```

Example: `$POV,T,15.0*07`

In XCSoar: port **USB serial**, driver **OpenVario**. InfoBox **Outside air temperature**.

Catch: Stefly's Android does not finish booting when the probe is plugged. The firmware waits 2 minutes before starting USB to work around that. I have not tested that delay. What works: unplug the probe until Android has booted, then plug it back in.

### Option 2 - HID (modified XCSoar)

Firmware: `firmware/hid/`. The XIAO is a vendor HID device, `2E8A:0A01`. Every 10 s it refreshes a 4-byte report: temperature in hundredths of a degree C, plus a status byte. Details: [../docs/hid-xcsoar.md](../docs/hid-xcsoar.md).

Stefly boots fine with it plugged. Stock XCSoar cannot read it. The XCSoar patch is in [../xcsoar/](../xcsoar/). See [07 - XCSoar modifications](07-xcsoar-modifications.md).

In XCSoar: port **USB HID**, driver **OAT HID**. InfoBox **Outside air temperature**.

## Cable

A **USB-C (XIAO) to USB-A** data cable is required for typical tablet USB-A ports.
