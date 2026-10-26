# OAT probe - build notes

Workshop steps for the USB OAT probe on a **Stefly Nav** / XCSoar. Story, costs, and why this stack: [../README.md](../README.md).

Hardware I used:

- Seeed **XIAO RP2040**
- Adafruit **ADA3648** PT1000 amplifier
- 2-wire class B **PT1000** (iOVEO 1 m)

Two options, same hardware:

- **Option 1 - serial.** Firmware emits `$POV,T,<degC>*CS` over USB serial. Stock XCSoar, driver **OpenVario**. Firmware waits 2 minutes before enabling USB so Android can boot (untested). Fallback: plug the probe after boot.
- **Option 2 - HID.** Firmware sends a vendor HID report. Probe stays plugged. Needs a modified XCSoar, driver **OAT HID**.

How to pick: [../README.md](../README.md).

![Parts overview](images/parts-gotronic.jpg)

---

## Follow in order

| Step | Document | Option | Goal |
|------|----------|--------|------|
| 0 | [00 - Overview](00-overview.md) | both | Understand the system |
| 1 | [01 - Bill of materials](01-bom.md) | both | Gather the parts |
| 2 | [02 - Prepare the amplifier](02-prepare-amplifier.md) | both | Wires into holes + 2-wire jumpers |
| 3 | [03 - Wiring](03-wiring.md) | both | Connect XIAO and ADA3648 |
| 4 | [04 - Firmware](04-firmware.md) | both | Flash `firmware/serial` or `firmware/hid` |
| 5 | [05 - Bench test](05-bench-test.md) | both | Verify output on a computer |
| 6 | [06 - XCSoar setup (serial)](06-xcsoar-stefly.md) | 1 | USB serial, OpenVario, 2-minute USB delay or plug after boot |
| 7 | [07 - XCSoar modifications](07-xcsoar-modifications.md) | 2 | What XCSoar needs to read the HID probe |
| 8 | [08 - XCSoar setup (HID)](08-xcsoar-stefly-hid.md) | 2 | USB HID, OAT HID driver |
| 9 | [09 - Mount & calibrate](09-mount-calibrate.md) | both | Install in the glider |

Option 1: do 0-6, then 9. Option 2: do 0-5, then 7-9.

---

## Photos

Build photos are manufacturer / retailer product pictures. See [images/SOURCES.md](images/SOURCES.md).

## Board docs

Offline datasheets / schematics: [resources/boards/](resources/boards/).

---

## Expected result

Option 1, on the serial port:

```text
$POV,T,23.52*35
```

Option 2, from `tools/read_oat_hid.py`:

```text
T=23.52 C  status=0
```

About every 10 seconds. XCSoar InfoBox **Outside air temperature**.
