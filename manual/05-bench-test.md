# 05 - Bench test (before Stefly)

Do this on a laptop so faults are easy to see.

## 1. Open the serial monitor

```bash
pio device monitor -b 115200
```

Or PlatformIO **Monitor**. Baud is ignored by USB CDC but 115200 matches XCSoar's setting.

## 2. What you should see

About once per second:

```text
# OAT probe ready - $POV,T for XCSoar OpenVario
# R=1079.32 Ohm  T=20.45 C
$POV,T,20.45*CS
```

- Lines starting with `#` are debug (humans only).
- Lines starting with `$POV,T,` are for XCSoar.

## 3. Sanity checks

| Observation | Likely cause |
|-------------|--------------|
| No serial port | Cable charge-only / wrong port / BOOT mode |
| `# FAULT: ... open` | Probe not connected, bad screw clamp, jumpers not closed |
| `# FAULT: ... short` | Wires shorted, wrong jumpers |
| Garbage temperature (+/- hundreds of C) | PT100 board used, or 2-wire jumpers open |
| Resistance ~1000 Ohm at room temp | Healthy PT1000 (roughly; room temp is higher than 1000 Ohm) |
| Resistance ~100 Ohm | You have a **PT100** probe - wrong for this firmware/amp |

Room-temperature PT1000 is typically roughly **1070-1100 Ohm**, not exactly 1000 Ohm (1000 Ohm is at 0 C).

## 4. Warm / cold test

1. Hold the probe tip - temperature should rise slowly.
2. Ice water (probe tip only, electronics dry) - should fall toward ~0 C.

2-wire leads add a small positive offset; see [07 - Mount & calibrate](07-mount-calibrate.md).

## Checklist

- [ ] Serial opens
- [ ] `$POV,T,...` every second
- [ ] No FAULT lines
- [ ] Temperature moves when you warm/cool the probe
