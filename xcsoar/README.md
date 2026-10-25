# XCSoar HID OAT driver

The XCSoar code that reads this probe over vendor USB HID (2E8A:0A01). See `../docs/hid-xcsoar.md` for the protocol.

It comes from [gabriel-briffe/XCSoar@e871280](https://github.com/gabriel-briffe/XCSoar/commit/e8712801d6cb598d222c450dbac89ba82c2e5d4a), rebased onto upstream [XCSoar master @797dc0b](https://github.com/XCSoar/XCSoar/commit/797dc0b74). Only the driver code is kept.

- `patches/0001-*.patch`: apply it to an upstream XCSoar master checkout with `git am`.
- Every other file here is a full copy of the patched file, at its XCSoar path, for reading.
