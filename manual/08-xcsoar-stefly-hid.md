# 08 - XCSoar setup, option 2 (HID)

Option 2 only. You need an XCSoar build with the HID driver from [07 - XCSoar modifications](07-xcsoar-modifications.md). Stock XCSoar will not see the probe.

Tested: **XCSoar Mac** and **XCSoar on an Android phone** (USB-C to USB-C), and **XCSoar on Stefly**. Not tried yet on other platforms.

## Hardware

Leave the probe plugged (rear **USB-A**, USB-C to USB-A cable). HID enumerates immediately. Android boots fine with it in.

1. Power off or leave Stefly on - either works.
2. Android may ask to open XCSoar / allow USB - accept.

## Device settings

In XCSoar:

1. **Config -> Devices** (or **Setup -> Devices**).
2. Pick a free device slot.
3. **Port:** the new **USB HID** entry (appears when the XIAO is enumerated).
4. **Driver:** **OAT HID**
5. No baud rate. It is not a serial port.
6. Enable the device / leave monitoring on.

## Show OAT

1. Edit an InfoBox page.
2. Add **Outside air temperature** (OAT).
3. You should see a live degC value updating about every 10 seconds.

## If the USB HID port does not appear

| Check | Action |
|-------|--------|
| Cable | Must support data |
| Identity | Firmware must be VID/PID `2E8A:0A01` (`firmware/hid/`). `000A` is the option 1 serial firmware. |
| XCSoar build | Must include the patch from [../xcsoar/](../xcsoar/) |

Still missing? On a PC, run `python3 tools/read_oat_hid.py`. If it cannot open `2E8A:0A01`, reflash `firmware/hid/`.

## Protocol reminder

A 4-byte HID report: report ID `1`, temperature as int16 hundredths of a degree C, status (`0` ok, `1` fault). Details: [docs/hid-xcsoar.md](../docs/hid-xcsoar.md).
