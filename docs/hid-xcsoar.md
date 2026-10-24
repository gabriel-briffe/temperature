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

## Mac emulator (no board)

macOS will not let a normal program pretend to be a USB HID gadget (Apple entitlement `com.apple.developer.hid.virtual.device`). So the desk test sends the **same 4 bytes** over TCP:

```bash
python3 tools/emulate_oat_hid.py --tcp
```

Listens on `127.0.0.1:4354`. Each message is exactly the HID report (`01 .. .. 00`). Temperature walks around 15 C.

In the XCSoar fork, for Mac desk testing, open that TCP socket and parse it with the same HID decoder (do not use OpenVario / `$POV`). On the Stefly / real XIAO, use USB HID `2E8A:0A01`.

Check the bytes without XCSoar:

```bash
python3 -c 'import socket,struct,time
s=socket.create_connection(("127.0.0.1",4354))
while True:
    b=s.recv(4)
    if len(b)<4: break
    rid,t,st=struct.unpack("<BhB", b)
    print(rid, t/100, st)'
```
