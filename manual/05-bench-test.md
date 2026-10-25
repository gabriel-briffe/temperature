# 05 - Bench test (before Stefly)

Do this on a laptop so faults are easy to see.

This `hid` branch has no serial port. There is no serial monitor and no `$POV,T` text. I test the HID report instead.

## 1. Read the HID report

Plug the XIAO into the laptop. Then:

```bash
pip install hidapi
python3 tools/read_oat_hid.py
```

The tool opens `2E8A:0A01` and prints each report.

## 2. What you should see

```text
reading HID reports (Ctrl+C to stop)
T=20.45 C  status=0
```

- `status=0` - good reading.
- `status=1` - sensor fault. The temperature is the last good value (or 0). Ignore it.
- A new value about every 10 seconds.

Report format: [docs/hid-xcsoar.md](../docs/hid-xcsoar.md).

## 3. Sanity checks

| Observation | Likely cause |
|-------------|--------------|
| `open 2E8A:0A01 failed` | Cable charge-only / BOOT mode / old firmware (`000A` is the serial build) |
| `status=1` | Probe not connected, bad screw clamp, wires shorted, wrong jumpers |
| Garbage temperature (+/- hundreds of C) | PT100 board used, or 2-wire jumpers open |
| Reads about -220 C at room temp | You have a **PT100** probe (~100 Ohm) - wrong for this firmware/amp |

Room-temperature PT1000 is typically roughly **1070-1100 Ohm**, not exactly 1000 Ohm (1000 Ohm is at 0 C).

## 4. Warm / cold test

1. Hold the probe tip - temperature should rise slowly.
2. Ice water (probe tip only, electronics dry) - should fall toward ~0 C.

2-wire leads add a small positive offset; see [08 - Mount & calibrate](08-mount-calibrate.md).

## No board? Desk test XCSoar

```bash
python3 tools/emulate_oat_hid.py --tcp
```

It sends the same 4-byte report on `127.0.0.1:4354`. Point XCSoar at TCP port **4354 (OAT HID)**. Details: [docs/hid-xcsoar.md](../docs/hid-xcsoar.md).

## Checklist

- [ ] `read_oat_hid.py` opens the probe
- [ ] A report about every 10 seconds
- [ ] `status=0`
- [ ] Temperature moves when you warm/cool the probe
