# USB OAT probe for Stefly / XCSoar

I wanted a temperature probe for the Stefly Nav for winter flights. Same hardware, two ways to talk to XCSoar. Pick one.

## Pick an option

| | Option 1 - serial | Option 2 - HID |
|---|---|---|
| XCSoar | Stock XCSoar, no changes | Modified XCSoar (patch in [xcsoar/](xcsoar/)) |
| USB identity | USB serial `2E8A:000A` | Vendor HID `2E8A:0A01` |
| Data | `$POV,T` at 115200, driver **OpenVario** | 4-byte HID report, driver **OAT HID** |
| Stefly boot | Android does not finish booting with the probe plugged. Plug it **after** boot. | Probe stays plugged. Boots fine. |
| Firmware | [firmware/serial/](firmware/serial/) | [firmware/hid/](firmware/hid/) |

- **Option 1** if you want stock XCSoar and don't mind plugging the probe after Android has booted. The firmware waits 2 minutes before it starts USB, to let Stefly boot with the probe plugged. I have **not tested** that delay.
- **Option 2** if you want the probe plugged in all the time. You need an XCSoar build with the HID driver: [docs/hid-xcsoar.md](docs/hid-xcsoar.md) and [manual/07-xcsoar-modifications.md](manual/07-xcsoar-modifications.md).

Step-by-step for both: **[manual/README.md](manual/README.md)**.

![Parts](manual/images/parts-gotronic.jpg)

## Why this stack

**PT1000, class B, 2-wire.** Platinum RTDs are stable in the cold range we care about. Class B is cheap and honest (about +/-0.3 C at 0 C). I picked PT1000 rather than PT100 because lead-wire error is ten times smaller in temperature for the same cable resistance — fine for a short 2-wire run into the cockpit. Probe I used: iOVEO `036HS02201`, 1 m silicone, stainless tip, [Amazon](https://www.amazon.fr/dp/B088V83LVY).

**ADA3648 (MAX31865).** The analog part is the hard part. This board is made for PT1000 (4.3 kOhm reference). Close the two "2 Wire" solder jumpers, screw the probe in, talk SPI. I soldered the PTFE straight into the logic holes (no 8-pin header). The RTD screw terminals were already on the board.

**XIAO RP2040.** Tiny, USB-C, cheap, Arduino. Option 1 makes it a USB serial device (`2E8A:000A`). Option 2 makes it a vendor HID device (`2E8A:0A01`). Either way the sensor is read every 10 s.

Between the two boards I used yellow **24 AWG PTFE** (UL 1213) from [Errebi](https://errebishop.com/fr/cordons-monobrin/7499-cable-style-1213-24-awg-teflon-ptfe-600v-jaune.html). Heat shrink over the joints (not counted in the price).

## What it cost me

Gotronic prices below are what I paid (HT / TTC). Probe price is the Amazon listing. Wire is sold by the metre.

| Part | Where | EUR |
|------|--------|-----|
| Seeed XIAO RP2040 `102010428` (no headers) | [Gotronic 37395](https://www.gotronic.fr/art-carte-xiao-rp2040-34095.htm) | 5.42 HT / **6.50 TTC** |
| Adafruit ADA3648 PT1000 amp | [Gotronic 35867](https://www.gotronic.fr/art-amplificateur-pour-pt1000-ada3648-27968.htm) | 16.62 HT / **19.95 TTC** |
| iOVEO PT1000 class B, 2-wire, 1 m | [Amazon B088V83LVY](https://www.amazon.fr/dp/B088V83LVY) | **9.62** |
| Alpha Wire UL1213 24 AWG PTFE, yellow | [Errebi, per metre](https://errebishop.com/fr/cordons-monobrin/7499-cable-style-1213-24-awg-teflon-ptfe-600v-jaune.html) | 4.06 HT / **4.95 TTC** / m |
| **Parts total** (1 m of wire, no shipping) | | **about 41 TTC** |

Not in that number: soldering iron, solder, heat shrink, USB-C to USB-A data cable I already had, shipping.

## Does it work?

Yes, both options. What I actually tested:

| Option | Tested on | Result |
|--------|-----------|--------|
| 1 - Serial, no fork (without the 2-minute delay) | **Pixel 9** (Android, USB-C to USB-C), **Stefly**, **macOS** | Live OAT in XCSoar on all three |
| 1 - Serial, with the 2-minute delay (`firmware/serial/`) | - | **Not tested yet** |
| 2 - HID, modified XCSoar | **Stefly** only | Probe stays plugged, Stefly boots normally, OAT shows as soon as XCSoar opens |

Option 1 as it sits in `firmware/serial/` includes the untested delay. Until I try it, boot Stefly with the probe unplugged, then plug it in.

Not tested yet on other platforms (Windows, iOS, Linux, Kobo, ...).

Ice-cube glass (probe tip only): **0.1 C**. Room air was about 15.5 C / 1060 Ohm, which is what a PT1000 should read.

This is a cockpit gadget, not a certified instrument. Offset is small enough that I have not put a correction in firmware.

## How to build it

Step-by-step notes I used: **[manual/README.md](manual/README.md)**.

From the repo root. Option 1:

```bash
pio run -d firmware/serial -t upload
pio device monitor -b 115200
```

Option 2 (BOOTSEL to flash; there is no serial port):

```bash
pio run -d firmware/hid -t upload
python3 tools/read_oat_hid.py
```

Desk tests without the board: `tools/emulate_oat.py` sends `$POV,T` over UDP 4353 (option 1). `tools/emulate_oat_hid.py --tcp` sends the HID report over TCP 4354 (option 2).
