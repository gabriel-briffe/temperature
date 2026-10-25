# 06 - XCSoar modifications

Stock XCSoar cannot read this probe. This step explains what I changed in XCSoar and where to find it.

## Why stock XCSoar does not work

XCSoar on Android reads USB accessories as USB serial ports. This probe has no serial port.

The XIAO enumerates as a vendor USB HID device:

| Field | Value |
|-------|-------|
| VID | `2E8A` |
| PID | `0A01` |
| Manufacturer | `OAT` |
| Product | `PT1000 OAT` |

XCSoar's USB serial code only claims serial-type interfaces from a list of known IDs. It never claims this HID interface, so no port ever connects and no temperature arrives.

## Why a new driver

The old firmware sent NMEA text (`$POV,T,...`) and XCSoar's OpenVario driver read it. That is gone.

Now the probe sends a 4-byte HID report. It refreshes it every 10 seconds. The report holds a report ID, the temperature in hundredths of a degree C, and a status byte. XCSoar has to read that report, decode the bytes, and check the status. No existing driver does that, so I wrote one: **OatHid**.

## Where the code is

Everything is in [../xcsoar/](../xcsoar/):

- [xcsoar/patches/](../xcsoar/patches/) - one patch. Apply it with `git am` on official XCSoar master (`797dc0b`).
- The other files in `xcsoar/` are full copies of each touched file, at their XCSoar paths. Read them, no need to apply anything.
- [docs/hid-xcsoar.md](../docs/hid-xcsoar.md) - USB identity and report format.

## What the patch touches

- **New driver** - `src/Device/Driver/OatHid/`: `Device.cpp` feeds XCSoar's outside air temperature, `Parser.cpp` / `Parser.hpp` decode the report.
- **Android side** - `OatHidPort.java` opens the HID interface and reads reports. `ClaimedUsbPort.java` lets serial and HID ports share the USB code. `usb_device_filter.xml` adds `2E8A:0A01` so Android offers XCSoar the probe.
- **Device config and registration** - a new port type `ANDROID_USB_HID`, and the driver is registered.
- **Port picker UI** - the HID port shows in the list, `ANDROID_USB_HID` sits beside `ANDROID_USB_SERIAL`, and TCP port **4354 (OAT HID)** is a choice for desk testing with the emulator.
- **Test** - `test/src/TestOatHid.cpp` checks the decoder.

It is smaller than it looks. The new files are about 400 lines (409). Most edits in existing files are one or two lines.

## Checklist

- [ ] Patch applied with `git am` on official XCSoar master
- [ ] XCSoar built for Android
- [ ] Probe plugged in: the HID port appears and the OAT InfoBox shows a value
