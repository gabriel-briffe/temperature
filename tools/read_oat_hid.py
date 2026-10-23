#!/usr/bin/env python3
"""Read vendor HID OAT reports from the XIAO (hid branch)."""

from __future__ import annotations

import struct
import sys
import time

VID = 0x2E8A
PID = 0x0A01


def main() -> int:
    try:
        import hid
    except ImportError:
        print("pip install hidapi", file=sys.stderr)
        return 1

    dev = hid.device()
    try:
        dev.open(VID, PID)
    except OSError as exc:
        print(f"open 2E8A:0A01 failed: {exc}", file=sys.stderr)
        return 1

    print("reading HID reports (Ctrl+C to stop)", flush=True)
    try:
        while True:
            data = dev.read(4, timeout_ms=1000)
            if not data:
                continue
            if len(data) < 4 or data[0] != 1:
                print(f"odd report: {bytes(data).hex()}", flush=True)
                continue
            temp_centi, status = struct.unpack_from("<hB", bytes(data), 1)
            print(f"T={temp_centi / 100.0:.2f} C  status={status}", flush=True)
    except KeyboardInterrupt:
        print()
    finally:
        dev.close()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
