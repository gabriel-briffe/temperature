# 01 - Bill of materials

Example retailer in France: **[Gotronic.fr](https://www.gotronic.fr/)** (any equivalent supplier works).

![Parts overview](images/parts-gotronic.jpg)

## Required

| Qty | Item | Notes / example links |
|-----|------|----------------------|
| 1 | Seeed **XIAO RP2040** (e.g. 102010428, without soldered headers) | [Gotronic](https://www.gotronic.fr/art-carte-xiao-rp2040-34095.htm) |
| 1 | Adafruit **ADA3648** PT1000 amplifier (MAX31865, 4.3 kOhm Rref) | [Gotronic](https://www.gotronic.fr/art-amplificateur-pour-pt1000-ada3648-27968.htm) |
| 1 | 2-wire **PT1000** probe | Any 1000 Ohm @ 0 C platinum RTD |
| 1 | USB-C to USB-A **data** cable | For the nav USB-A port |

The ADA3648 kit typically includes (unsoldered): 8-pin header + two 2-pin screw terminals.

The XIAO 102010428 variant ships **without soldered headers** - solder pin headers before wiring.

## Also needed

| Item | Why |
|------|-----|
| Solder + iron | Headers, 2-wire jumpers on ADA3648 |
| Hookup / Dupont wires | SPI + power between boards |
| Breadboard (optional) | First bench test |

## Do not substitute

| Wrong | Why |
|-------|-----|
| ADA3328 (PT100 amp) | Different Rref - not for PT1000 |
| PT100 probe on ADA3648 | Firmware expects PT1000 |
| Charge-only USB cable | No data to XCSoar |

## Confirm PT1000 amp

Silkscreen / bag: **ADA3648** / Adafruit **3648**, not 3328.
