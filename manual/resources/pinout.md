# Pin reference

## Seeed XIAO RP2040 (relevant pins)

USB-C at the top. Looking at the top of the board:

```text
                 USB-C
        +-------------------+
   D0   |                   |  5V
   D1   |                   |  GND
   D2   |     XIAO RP2040   |  3V3
   D3   |                   |  D10  MOSI  -> SDI
   D4   |                   |  D9   MISO  <- SDO
   D5   |                   |  D8   SCK   -> CLK
   D6   |                   |  D7   CS    -> CS
        +-------------------+
```

| XIAO label | GPIO (typical) | Use in this project |
|------------|----------------|---------------------|
| 3V3 | - | Power to ADA3648 Vin |
| GND | - | Ground |
| D8 | GPIO2 | SPI SCK -> CLK |
| D9 | GPIO4 | SPI MISO <- SDO |
| D10 | GPIO3 | SPI MOSI -> SDI |
| D7 | GPIO1 | CS |

## ADA3648 breakout (logic header)

| Pad | Direction | Connect to |
|-----|-----------|------------|
| Vin | power in | XIAO 3V3 |
| 3Vo | 3.3 V out | unused |
| GND | ground | XIAO GND |
| CLK | in | XIAO D8 |
| SDO | out | XIAO D9 |
| SDI | in | XIAO D10 |
| CS | in | XIAO D7 |

## ADA3648 RTD terminals

Four screws: **left pair** and **right pair**.

- 2-wire: solder both triangular jumpers; one probe wire left, one right.
- See [02 - Prepare the amplifier](../02-prepare-amplifier.md) for the annotated photo.
