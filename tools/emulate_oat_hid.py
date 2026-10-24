#!/usr/bin/env python3
"""
Fake HID OAT reports for an XCSoar Mac fork.

  python3 tools/emulate_oat_hid.py           # virtual HID 2E8A:0A01 (macOS)
  python3 tools/emulate_oat_hid.py --tcp     # raw 4-byte reports on 127.0.0.1:4354

TCP mode is the reliable desk test: same bytes as the XIAO HID report
(id, int16 LE hundredths C, status). Point the fork at that socket if
IOHIDUserDevice is blocked (Apple virtual-HID entitlement).
"""

from __future__ import annotations

import argparse
import math
import os
import socket
import stat
import struct
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC = os.path.join(ROOT, "tools", "emulate_oat_hid.c")
BIN = os.path.join(ROOT, "tools", "emulate_oat_hid")

VID = 0x2E8A
PID = 0x0A01


def pack_report(celsius: float, status: int = 0) -> bytes:
    centi = int(round(celsius * 100.0))
    centi = max(-32768, min(32767, centi))
    return struct.pack("<BhB", 1, centi, status)


def tcp_loop(host: str, port: int, interval: float, base_temp: float) -> int:
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((host, port))
    sock.listen(1)
    sock.settimeout(0.5)
    print(f"HID OAT TCP {host}:{port}  4-byte reports  (wait for XCSoar)", flush=True)
    print("Fork: connect TCP and read 4 bytes (same as HID report).", flush=True)

    conn = None
    start = time.monotonic()
    try:
        while True:
            if conn is None:
                try:
                    conn, addr = sock.accept()
                    conn.settimeout(0.0)
                    print(f"client {addr}", flush=True)
                except socket.timeout:
                    continue
            elapsed = time.monotonic() - start
            temp = base_temp + 2.0 * math.sin(elapsed / 60.0)
            report = pack_report(temp)
            try:
                conn.sendall(report)
            except OSError:
                print("client gone", flush=True)
                conn.close()
                conn = None
                continue
            print(
                f"T={temp:.2f} C  report={report.hex(' ').upper()}",
                flush=True,
            )
            time.sleep(interval)
    except KeyboardInterrupt:
        print("\nStopped.", flush=True)
    finally:
        if conn is not None:
            conn.close()
        sock.close()
    return 0


def need_build() -> bool:
    if not os.path.isfile(BIN):
        return True
    return os.path.getmtime(SRC) > os.path.getmtime(BIN)


def build() -> None:
    cmd = [
        "clang",
        "-o",
        BIN,
        SRC,
        "-framework",
        "IOKit",
        "-framework",
        "CoreFoundation",
    ]
    print(" ".join(cmd), flush=True)
    subprocess.check_call(cmd)
    os.chmod(BIN, os.stat(BIN).st_mode | stat.S_IXUSR)


def main() -> int:
    p = argparse.ArgumentParser(description="Emulate HID OAT probe for XCSoar Mac")
    p.add_argument("--interval", type=float, default=2.0)
    p.add_argument("--temp", type=float, default=15.0)
    p.add_argument(
        "--tcp",
        nargs="?",
        const="127.0.0.1:4354",
        metavar="HOST:PORT",
        help="send raw HID reports over TCP (default 127.0.0.1:4354)",
    )
    args = p.parse_args()

    if args.tcp:
        host, _, port_s = args.tcp.partition(":")
        return tcp_loop(host or "127.0.0.1", int(port_s or "4354"), args.interval, args.temp)

    if sys.platform != "darwin":
        print("Virtual HID is macOS only. Use --tcp.", file=sys.stderr)
        return 1

    if need_build():
        try:
            build()
        except (OSError, subprocess.CalledProcessError) as exc:
            print(f"build failed: {exc}", file=sys.stderr)
            print("falling back to --tcp", file=sys.stderr)
            return tcp_loop("127.0.0.1", 4354, args.interval, args.temp)

    raise SystemExit(subprocess.call([BIN, str(args.interval), str(args.temp)]))


if __name__ == "__main__":
    raise SystemExit(main())
