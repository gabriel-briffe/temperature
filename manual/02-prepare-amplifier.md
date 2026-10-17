# 02 - Prepare the amplifier (ADA3648)

Kit contents (product photo):

![ADA3648 kit](images/ada3648-gotronic.jpg)

Do this **before** wiring to the XIAO.

## 1. Solder the 8-pin header

Solder the male header to the bottom row: `Vin`, `3Vo`, `GND`, `CLK`, `SDO`, `SDI`, `CS`, `RDY`.

## 2. Solder the RTD terminal blocks

Solder both 2-pin screw terminals onto the top RTD holes (`F-` / `RTD-` / `RTD+` / `F+`).

## 3. Configure for 2-wire (critical)

Board defaults to 4-wire. For a 2-wire probe:

1. Solder-bridge **both** triangular **"2 Wire"** pads closed (under the terminal area on the photo above).
2. Alternate: short the two left pads together and the two right pads together with wire.

## 4. Attach the PT1000

1. One probe wire into a **left** terminal, tighten.
2. Other probe wire into a **right** terminal, tighten.
3. Polarity does not matter. Tug-test both.

## Checklist

- [ ] Header soldered
- [ ] Terminal blocks soldered
- [ ] Both 2-wire jumpers closed
- [ ] Probe clamped left + right
