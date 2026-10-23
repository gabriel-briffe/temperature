# HID OAT for an XCSoar fork

This `hid` branch does **not** speak `$POV,T` serial. The XIAO is a vendor HID device from power-on (same USB family as the Stefly stick, not a mouse).

Stock XCSoar OpenVario will not see a temperature. Your fork has to read the report below.

## USB identity

| Field | Value |
|-------|--------|
| VID | `0x2E8A` |
| PID | `0x0A01` (not `000A` — that PID is Pico CDC) |
| Class | HID (3), protocol none |
| Usage page | `0xFF` |
| Usage | `0x01` |

## Report (interrupt IN)

Every 10 s the probe refreshes this. The host may poll as often as every 255 ms; unused polls repeat the last report.

| Offset | Type | Meaning |
|--------|------|---------|
| 0 | `uint8` | Report ID `1` |
| 1-2 | `int16` LE | Hundredths of a degC. `1550` = 15.50 C |
| 3 | `uint8` | `0` = ok, `1` = sensor fault |

Example: `01 0E 06 00` -> 15.50 C, good.

On fault, status is `1`. Temperature is the last good value (or 0 if none yet). Ignore the number when status is `1`.

## XCSoar fork

1. **Port** — find VID/PID `2E8A:0A01`, claim the HID interface, read interrupt IN. On Android: `UsbManager` (do not use the serial allowlist). Request USB permission. Because this is not a boot mouse, the system should let the app claim it.
2. **Driver** — parse the 4-byte report. If status is 0: `info.temperature = raw / 100.0` and mark valid (same sink as OpenVario type `T`).
3. **Device list** — new driver name, e.g. "OAT HID". No baud.
4. **InfoBox** — unchanged: Outside air temperature.

Mac check without XCSoar:

```bash
python3 tools/read_oat_hid.py
```

(needs `pip install hidapi`)
