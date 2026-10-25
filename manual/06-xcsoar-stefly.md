# 06 - XCSoar setup, option 1 (serial)

Option 1 only. Stock XCSoar, no changes. For option 2, skip to [07 - XCSoar modifications](07-xcsoar-modifications.md).

## The boot problem

With the serial probe plugged in, Stefly's Android does not finish booting.

To get around it, the firmware stays off USB for **2 minutes** after power, then starts the serial port. I have **not tested** that delay on the Stefly yet.

What I know works: unplug the probe until Android has finished booting, then plug it back in.

## Hardware

1. Boot the Stefly with the probe **unplugged**.
2. Wait until Android and XCSoar are up.
3. Plug the probe (rear **USB-A**, USB-C to USB-A cable).
4. Wait for the serial port. The firmware still waits its 2 minutes after it gets power.
5. Android may ask to open XCSoar / allow USB - accept.

## Device settings

In XCSoar:

1. **Config -> Devices** (or **Setup -> Devices**).
2. Pick a free device slot.
3. **Port:** the new **USB serial** entry (appears when the XIAO is enumerated).
4. **Driver:** **OpenVario**
5. **Baud rate:** **115200** (framing is fixed 8N1; baud is largely unused for CDC but set it anyway).
6. Enable the device / leave monitoring on.

## Show OAT

1. Edit an InfoBox page.
2. Add **Outside air temperature** (OAT).
3. You should see a live degC value updating about every 10 seconds.

## If the USB port does not appear

| Check | Action |
|-------|--------|
| Waited? | The port only appears 2 minutes after the probe gets power |
| Cable | Must support data |
| Allowlist | Firmware must use VID/PID `2E8A:000A` (`firmware/serial/`) |
| Wrong firmware | `2E8A:0A01` is the option 2 HID firmware. Reflash `firmware/serial/` |
| Device already used | Unplug other USB-serial gadgets and retry |
| XCSoar version | Use a recent build that includes the Pico SoftRF Lego USB IDs |

Still missing? On a PC, confirm `lsusb` / Device Manager shows `2E8A:000A`.

## Protocol reminder

Only this is required for OAT:

```text
$POV,T,12.34*CS
```

No pressure/vario fields needed. OpenVario ignores unknown types; temperature type is `T`.
