# USB OAT probe for Stefly / XCSoar

I wanted a temperature probe for the Stefly Nav for winter flights. Here is a solution that works so far.

This **`hid` branch** sends temperature as a vendor **HID** report (not `$POV,T` serial). Android can boot with the cable already in. Stock XCSoar will not see OAT — a fork has to read the report. Details: [docs/hid-xcsoar.md](docs/hid-xcsoar.md).

`main` still speaks OpenVario serial (`$POV,T`), with a 2 minute USB delay.

![Parts](manual/images/parts-gotronic.jpg)

## Why this stack

**PT1000, class B, 2-wire.** Platinum RTDs are stable in the cold range we care about. Class B is cheap and honest (about +/-0.3 C at 0 C). I picked PT1000 rather than PT100 because lead-wire error is ten times smaller in temperature for the same cable resistance — fine for a short 2-wire run into the cockpit. Probe I used: iOVEO `036HS02201`, 1 m silicone, stainless tip, [Amazon](https://www.amazon.fr/dp/B088V83LVY).

**ADA3648 (MAX31865).** The analog part is the hard part. This board is made for PT1000 (4.3 kOhm reference). Close the two "2 Wire" solder jumpers, screw the probe in, talk SPI. I soldered the PTFE straight into the logic holes (no 8-pin header). The RTD screw terminals were already on the board.

**XIAO RP2040.** Tiny, USB-C, cheap, Arduino. This branch is vendor HID `2E8A:0A01` (not Pico serial `000A`). It enumerates immediately. Sensor still read every 10 s.

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

Yes. I have seen live OAT in **XCSoar on Mac**, **XCSoar on an Android phone** (USB-C to USB-C), and **XCSoar on Stefly**. Not tested yet on other platforms (Windows, iOS, Linux, Kobo, …).

Ice-cube glass (probe tip only): **0.1 C**. Room air was about 15.5 C / 1060 Ohm, which is what a PT1000 should read.

This is a cockpit gadget, not a certified instrument. Offset is small enough that I have not put a correction in firmware.

## How to build it

Step-by-step notes I used: **[manual/README.md](manual/README.md)**.

From the repo root (BOOTSEL to flash; there is no serial port):

```bash
pio run -t upload
python3 tools/read_oat_hid.py
```

Desk test without the board (same 4-byte report over TCP):

```bash
python3 tools/emulate_oat_hid.py --tcp
```

Firmware is `src/main.cpp`. XCSoar fork: HID driver, see [docs/hid-xcsoar.md](docs/hid-xcsoar.md).
