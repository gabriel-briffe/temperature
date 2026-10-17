#!/usr/bin/env python3
"""
Fake OAT probe for XCSoar — blind emit (like the real USB board).

Sends $POV,T,<celsius>*CS to UDP 127.0.0.1:4353 every few seconds.

XCSoar (not in simulator mode):
  Devices -> Port = "UDP port", port 4353, driver OpenVario
"""

from __future__ import annotations

import argparse
import math
import socket
import sys
import time


def nmea_checksum(body: str) -> str:
    cs = 0
    for ch in body:
        cs ^= ord(ch)
    return f"{cs:02X}"


def oat_line(celsius: float) -> str:
    body = f"POV,T,{celsius:.2f}"
    return f"${body}*{nmea_checksum(body)}\r\n"


def main() -> int:
    p = argparse.ArgumentParser(description="Emulate PT1000 OAT probe for XCSoar")
    p.add_argument("--host", default="127.0.0.1")
    p.add_argument("--port", type=int, default=4353)
    p.add_argument("--interval", type=float, default=2.0)
    p.add_argument("--temp", type=float, default=15.0)
    args = p.parse_args()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    print(f"UDP OAT -> {args.host}:{args.port} every {args.interval:g}s (blind)",
          flush=True)
    print("XCSoar: UDP port {0}, driver OpenVario (not sim mode)".format(args.port),
          flush=True)

    start = time.monotonic()
    try:
        while True:
            elapsed = time.monotonic() - start
            temp = args.temp + 2.0 * math.sin(elapsed / 60.0)
            line = oat_line(temp)
            sock.sendto(line.encode("ascii"), (args.host, args.port))
            print(line.strip(), flush=True)
            time.sleep(args.interval)
    except KeyboardInterrupt:
        print("\nStopped.", flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
