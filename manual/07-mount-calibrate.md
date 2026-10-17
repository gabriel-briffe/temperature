# 07 - Mount & calibrate

## Probe placement (soaring)

OAT must see **outside air**, not cabin heat or exhaust:

- Mount the tip in clear airflow (fuselage side, under wing, boom - depending on aircraft).
- Avoid direct sun on the tip if possible (radiation error).
- Keep the electronics (XIAO + amp) in the cockpit / instrument bay.
- Strain-relieve the probe cable so vibration cannot pull the terminal screws.

## Why 2-wire needs a small correction

Each lead wire adds resistance. The amp cannot cancel that in 2-wire mode, so the reported temperature is often **slightly high**.

Example: 2 Ohm total lead resistance on a PT1000 ~ a few tenths of a degC (order of magnitude; depends on cable).

## Ice-bath check (optional)

1. Mix crushed ice + a little water; wait until stable.
2. Immerse **only the probe tip** (not the amplifier).
3. Note the reported temperature `T_ice`.
4. Ideal is 0.0 C. Offset ~ `T_ice`.

To apply a fixed offset in firmware later, subtract that constant in `emitOat()`.

## Mechanical checklist for flight

- [ ] Probe secure, tip in free air
- [ ] Cable secured
- [ ] USB cable strain-relieved at Stefly
- [ ] Bench `$POV,T` still good after final cable lengths
- [ ] OAT InfoBox verified on the ground before takeoff
