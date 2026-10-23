# 04 - Firmware (flash the XIAO)

Firmware lives at the repo root (`platformio.ini`, `src/main.cpp`). It reads the PT1000 over SPI and prints `$POV,T,...` on USB about every 10 seconds.

## Install PlatformIO

### Option A - VS Code / Cursor

1. Install the **PlatformIO IDE** extension.
2. Open the `temperature` folder (repo root, not only `manual/`).
3. Wait until PlatformIO finishes installing the RP2040 platform on first build.

### Option B - CLI

```bash
# macOS / Linux (example)
pipx install platformio
# or: brew install platformio

cd /path/to/temperature
pio run
```

## Put the XIAO in upload mode (if needed)

Plug the XIAO into the **computer** first (USB-C data cable), not the Stefly.

The probe stays off USB for **2 minutes** after power so a Stefly can boot with the cable already plugged. For a flash, use BOOTSEL (below) or wait until the serial port appears.

1. Hold the **BOOT** button.
2. Press and release **RESET** (or unplug, hold BOOT, plug USB back in).
3. Release **BOOT**.
4. A USB drive named something like `RPI-RP2` should appear. Then upload.

## Build & upload

From the **repo root** (the folder that contains `platformio.ini`, not `manual/`):

```bash
pio run -t upload
```

Or in the IDE: **Upload** button.

## USB identity (already set)

`platformio.ini` forces:

- VID `0x2E8A`
- PID `0x000A`

so Stefly / XCSoar Android recognize the device.

## Libraries

Pulled automatically via `lib_deps`:

- Adafruit MAX31865 library
