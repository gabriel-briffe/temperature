# 01 - Bill of materials

What I actually bought. Retailer in France for the boards: **[Gotronic.fr](https://www.gotronic.fr/)**.

![Parts overview](images/parts-gotronic.jpg)

## Parts

| Qty | Item | I paid | Link |
|-----|------|--------|------|
| 1 | Seeed **XIAO RP2040** `102010428` (no headers) | 5.42 HT / 6.50 TTC | [Gotronic 37395](https://www.gotronic.fr/art-carte-xiao-rp2040-34095.htm) |
| 1 | Adafruit **ADA3648** PT1000 amp (MAX31865, 4.3 kOhm Rref) | 16.62 HT / 19.95 TTC | [Gotronic 35867](https://www.gotronic.fr/art-amplificateur-pour-pt1000-ada3648-27968.htm) |
| 1 | 2-wire **PT1000** class B, 1 m (iOVEO `036HS02201`) | 9.62 | [Amazon](https://www.amazon.fr/dp/B088V83LVY) |
| ~1 m | 24 AWG PTFE UL1213 (board-to-board) | 4.95 TTC / m | [Errebi](https://errebishop.com/fr/cordons-monobrin/7499-cable-style-1213-24-awg-teflon-ptfe-600v-jaune.html) |
| 1 | USB-C to USB-A **data** cable | already had | Stefly USB-A port |

ADA3648 kit usually includes an 8-pin header + two 2-pin screw terminals. On mine the RTD blocks were already soldered; I skipped the header and soldered wires into the holes.

XIAO `102010428` ships **without** soldered headers — I soldered the same wires to the pads.

Parts total about **41 EUR TTC** if you buy 1 m of wire, plus shipping.

## Tools I already had (not in the price)

| Item | Why |
|------|-----|
| Soldering iron + solder | Wires into holes, 2-wire jumpers |
| Heat shrink | Insulate the joints |
| Wire strippers | PTFE is stiff; strip carefully |

## Do not substitute

| Wrong | Why |
|-------|-----|
| ADA3328 (PT100 amp) | Different Rref — not for PT1000 |
| PT100 probe on ADA3648 | Firmware expects PT1000 |
| Charge-only USB cable | No data to XCSoar |

## Confirm PT1000 amp

Silkscreen / bag: **ADA3648** / Adafruit **3648**, not 3328.
