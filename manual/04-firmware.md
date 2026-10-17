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

1. Hold the **BOOT** button.
2. Press and release **RESET** (or plug USB while holding BOOT).
3. Release **BOOT**.
4. A USB drive named something like `RPI-RP2` may appear - PlatformIO can also upload via UF2/serial automatically.

## Build & upload

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

No manual Arduino Library Manager steps if you use PlatformIO.

## Arduino IDE (alternative)

If you prefer Arduino IDE instead of PlatformIO:

1. Install **Earle Philhower** Arduino-Pico core / Seeed XIAO RP2040 board package.
2. Install library **Adafruit MAX31865**.
3. Open `src/main.cpp` (rename/copy to `.ino` sketch if required).
4. Board: **Seeed XIAO RP2040**.
5. Set USB VID/PID to `2E8A` / `000A` if your board package exposes those options (otherwise prefer PlatformIO for Stefly compatibility).
