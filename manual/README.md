# OAT probe - build manual

Outside air temperature (OAT) probe for **XCSoar** (e.g. on a Stefly Nav or similar Android device).

Hardware:

- Seeed **XIAO RP2040**
- Adafruit **ADA3648** PT1000 amplifier
- 2-wire **PT1000** probe

Firmware emits `$POV,T,<degC>*CS` over USB. XCSoar driver: **OpenVario**.

![Parts overview](images/parts-gotronic.jpg)

---

## Follow in order

| Step | Document | Goal |
|------|----------|------|
| 0 | [00 - Overview](00-overview.md) | Understand the system |
| 1 | [01 - Bill of materials](01-bom.md) | Gather the parts |
| 2 | [02 - Prepare the amplifier](02-prepare-amplifier.md) | Solder headers + 2-wire jumpers |
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

Once per second. XCSoar InfoBox **Outside air temperature**.
