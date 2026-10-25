# 04 - Firmware (flash the XIAO)

Two firmwares. Pick the folder for your option:

| Option | Folder | What it sends |
|--------|--------|---------------|
| 1 - serial | `firmware/serial/` | `$POV,T` over USB serial, every 10 s, after a 2 minute USB delay |
| 2 - HID | `firmware/hid/` | Vendor HID report, refreshed every 10 s |

Each folder has its own `platformio.ini` and `src/main.cpp`.

## Install PlatformIO

### Option A - VS Code / Cursor

1. Install the **PlatformIO IDE** extension.
2. Open `firmware/serial` or `firmware/hid` (the folder with `platformio.ini`).
3. Wait until PlatformIO finishes installing the RP2040 platform on first build.

### Option B - CLI

```bash
# macOS / Linux (example)
pipx install platformio
# or: brew install platformio

cd /path/to/temperature
pio run -d firmware/serial   # option 1
pio run -d firmware/hid      # option 2
```

## Put the XIAO in upload mode

Plug the XIAO into the **computer** first (USB-C data cable), not the Stefly.

Option 1 stays off USB for 2 minutes after power. Option 2 has no serial port. So I use BOOTSEL for both:

1. Hold the **BOOT** button.
2. Press and release **RESET** (or unplug, hold BOOT, plug USB back in).
3. Release **BOOT**.
4. A USB drive named something like `RPI-RP2` should appear. Then upload.

## Build & upload

From the repo root:

```bash
pio run -d firmware/serial -t upload   # option 1
pio run -d firmware/hid -t upload      # option 2
```

Or in the IDE, with the right folder open: **Upload** button.

## USB identity (already set)

Option 1 (`firmware/serial/platformio.ini`) asks for:

- VID `0x2E8A`
- PID `0x000A` (Pico CDC, already on XCSoar's Android USB list as SoftRF Lego)

Option 2 (`firmware/hid/platformio.ini`) sets, via Earle Philhower board keys:

- VID `0x2E8A`
- PID `0x0A01`
- Manufacturer `OAT`
- Product `PT1000 OAT`

## Libraries

Pulled automatically via `lib_deps`:

- Adafruit MAX31865 library
