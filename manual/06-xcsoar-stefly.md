# 06 - XCSoar on Stefly Nav

Tested: **XCSoar Mac** and **XCSoar on an Android phone** (USB-C to USB-C), and **XCSoar on Stefly**. Not tried yet on other platforms.

## Hardware

Leave the probe plugged (rear **USB-A**, USB-C to USB-A cable). HID enumerates immediately. Stock XCSoar OpenVario will not see it — use the HID driver in [docs/hid-xcsoar.md](../docs/hid-xcsoar.md).

1. Power off or leave Stefly on - either works.
2. Android may ask to open XCSoar / allow USB - accept.

## Device settings

In XCSoar:

1. **Config -> Devices** (or **Setup -> Devices**).
2. Pick a free device slot.
3. **Port:** the new **USB serial** entry (appears when the XIAO is enumerated).
4. **Driver:** **OpenVario**
5. **Baud rate:** **115200** (framing is fixed 8N1; baud is largely unused for CDC but set it anyway).
6. Enable the device / leave monitoring on.

## Show OAT

1. Edit an InfoBox page.
2. Add **Outside air temperature** (OAT).
3. You should see a live degC value updating about every 10 seconds.

## If the USB port does not appear

| Check | Action |
|-------|--------|
| Cable | Must support data |
| Allowlist | Firmware must use VID/PID `2E8A:000A` (this repo does) |
| Device already used | Unplug other USB-serial gadgets and retry |
| XCSoar version | Use a recent build that includes the Pico SoftRF Lego USB IDs |

Still missing? On a PC, confirm `lsusb` / Device Manager shows `2E8A:000A`. If the XIAO shows a different ID, reflash with this project's `platformio.ini`.

## Protocol reminder

Only this is required for OAT:

```text
$POV,T,12.34*CS
```

No pressure/vario fields needed. OpenVario ignores unknown types; temperature type is `T`.
