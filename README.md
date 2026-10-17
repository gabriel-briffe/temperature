# Temperature - XCSoar OAT probe

USB outside-air-temperature probe for **XCSoar**: XIAO RP2040 + Adafruit ADA3648 + 2-wire PT1000.

## Build guide

Start here -> **[manual/README.md](manual/README.md)**

## Firmware (short)

```bash
pio run -t upload
pio device monitor -b 115200
```

Emits `$POV,T,<degC>*CS` - XCSoar driver **OpenVario**.
