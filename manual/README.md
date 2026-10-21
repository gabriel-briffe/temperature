# OAT probe - build notes

Workshop steps for the USB OAT probe on a **Stefly Nav** / XCSoar. Story, costs, and why this stack: [../README.md](../README.md).

Hardware I used:

- Seeed **XIAO RP2040**
- Adafruit **ADA3648** PT1000 amplifier
- 2-wire class B **PT1000** (iOVEO 1 m)

Firmware emits `$POV,T,<degC>*CS` over USB. XCSoar driver: **OpenVario**.

![Parts overview](images/parts-gotronic.jpg)

---

## Follow in order

| Step | Document | Goal |
|------|----------|------|
| 0 | [00 - Overview](00-overview.md) | Understand the system |
| 1 | [01 - Bill of materials](01-bom.md) | Gather the parts |
| 2 | [02 - Prepare the amplifier](02-prepare-amplifier.md) | Wires into holes + 2-wire jumpers |
| 3 | [03 - Wiring](03-wiring.md) | Connect XIAO and ADA3648 |
| 4 | [04 - Firmware](04-firmware.md) | Install tools and flash |
| 5 | [05 - Bench test](05-bench-test.md) | Verify NMEA on a computer |
| 6 | [06 - XCSoar setup](06-xcsoar-stefly.md) | Configure the device and show OAT |
| 7 | [07 - Mount & calibrate](07-mount-calibrate.md) | Install in the glider |

---

## Photos

Build photos are manufacturer / retailer product pictures. See [images/SOURCES.md](images/SOURCES.md).

## Board docs

Offline datasheets / schematics: [resources/boards/](resources/boards/).

---

## Expected result

```text
$POV,T,23.52*35
```

About every 10 seconds. XCSoar InfoBox **Outside air temperature**.
