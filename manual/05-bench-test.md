# 05 - Bench test (before Stefly)

Do this on a laptop so faults are easy to see. Follow the part for your option.

## Option 1 - serial

### 1. Open the serial monitor

```bash
pio device monitor -b 115200
```

Or PlatformIO **Monitor**. Baud is ignored by USB CDC but 115200 matches XCSoar's setting.

### 2. What you should see

After power the probe stays off USB for **2 minutes**. Then the serial port appears and, about once every 10 seconds:

```text
# OAT probe ready - $POV,T for XCSoar OpenVario
# R=1079.32 Ohm  T=20.45 C
$POV,T,20.45*CS
```

- Lines starting with `#` are debug (humans only).
- Lines starting with `$POV,T,` are for XCSoar.

### 3. Sanity checks

| Observation | Likely cause |
|-------------|--------------|
| No serial port after 2 minutes | Cable charge-only / wrong port / BOOT mode |
| `# FAULT: ... open` | Probe not connected, bad screw clamp, jumpers not closed |
| `# FAULT: ... short` | Wires shorted, wrong jumpers |
| Garbage temperature (+/- hundreds of C) | PT100 board used, or 2-wire jumpers open |
| Resistance ~100 Ohm | You have a **PT100** probe - wrong for this firmware/amp |

### No board? Desk test XCSoar

```bash
python3 tools/emulate_oat.py
```

It sends `$POV,T` to UDP `127.0.0.1:4353`. In XCSoar: port **UDP port** 4353, driver **OpenVario**.

## Option 2 - HID

There is no serial port and no `$POV,T` text. I read the HID report instead.

### 1. Read the HID report

Plug the XIAO into the laptop. Then:

```bash
pip install hidapi
python3 tools/read_oat_hid.py
```

The tool opens `2E8A:0A01` and prints each report.

### 2. What you should see

```text
reading HID reports (Ctrl+C to stop)
T=20.45 C  status=0
```

- `status=0` - good reading.
- `status=1` - sensor fault. The temperature is the last good value (or 0). Ignore it.
- A new value about every 10 seconds.

Report format: [docs/hid-xcsoar.md](../docs/hid-xcsoar.md).

### 3. Sanity checks

| Observation | Likely cause |
|-------------|--------------|
| `open 2E8A:0A01 failed` | Cable charge-only / BOOT mode / option 1 firmware flashed |
| `status=1` | Probe not connected, bad screw clamp, wires shorted, wrong jumpers |
| Garbage temperature (+/- hundreds of C) | PT100 board used, or 2-wire jumpers open |
| Reads about -220 C at room temp | You have a **PT100** probe (~100 Ohm) - wrong for this firmware/amp |

### No board? Desk test XCSoar

```bash
python3 tools/emulate_oat_hid.py --tcp
```

It sends the same 4-byte report on `127.0.0.1:4354`. Point the modified XCSoar at TCP port **4354 (OAT HID)**. Details: [docs/hid-xcsoar.md](../docs/hid-xcsoar.md).

## Both options

Room-temperature PT1000 is typically roughly **1070-1100 Ohm**, not exactly 1000 Ohm (1000 Ohm is at 0 C).

### Warm / cold test

1. Hold the probe tip - temperature should rise slowly.
2. Ice water (probe tip only, electronics dry) - should fall toward ~0 C.

2-wire leads add a small positive offset; see [09 - Mount & calibrate](09-mount-calibrate.md).

## Checklist

Option 1:

- [ ] Serial opens after 2 minutes
- [ ] `$POV,T,...` about every 10 seconds
- [ ] No FAULT lines

Option 2:

- [ ] `read_oat_hid.py` opens the probe
- [ ] A report about every 10 seconds
- [ ] `status=0`

Both:

- [ ] Temperature moves when you warm/cool the probe
