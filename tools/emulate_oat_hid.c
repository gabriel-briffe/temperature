/*
 * Virtual HID OAT probe for macOS (same reports as the hid firmware branch).
 *
 * Requires the modern IOHIDUserDevice API. Apple may require the
 * com.apple.developer.hid.virtual.device entitlement; if create fails,
 * use the TCP fallback in tools/emulate_oat_hid.py --tcp.
 */

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/hidsystem/IOHIDUserDevice.h>
#include <dispatch/dispatch.h>
#include <mach/mach_time.h>
#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const uint8_t kReportDesc[] = {
    0x05, 0xFF, 0x09, 0x01, 0xA1, 0x01, 0x85, 0x01, 0x09, 0x02, 0x16, 0x00,
    0x80, 0x26, 0xFF, 0x7F, 0x75, 0x10, 0x95, 0x01, 0x81, 0x02, 0x09, 0x03,
    0x15, 0x00, 0x26, 0xFF, 0x00, 0x75, 0x08, 0x95, 0x01, 0x81, 0x02, 0xC0};

static volatile sig_atomic_t g_run = 1;

static void on_sigint(int sig) {
  (void)sig;
  g_run = 0;
}

static void cf_set_int(CFMutableDictionaryRef dict, CFStringRef key, int value) {
  CFNumberRef num = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &value);
  CFDictionarySetValue(dict, key, num);
  CFRelease(num);
}

int main(int argc, char **argv) {
  double interval_s = 2.0;
  double base_temp = 15.0;
  if (argc > 1) {
    interval_s = atof(argv[1]);
    if (interval_s < 0.2) {
      interval_s = 0.2;
    }
  }
  if (argc > 2) {
    base_temp = atof(argv[2]);
  }

  CFMutableDictionaryRef props = CFDictionaryCreateMutable(
      kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks,
      &kCFTypeDictionaryValueCallBacks);
  if (!props) {
    fprintf(stderr, "CFDictionaryCreateMutable failed\n");
    return 1;
  }

  CFDataRef desc =
      CFDataCreate(kCFAllocatorDefault, kReportDesc, (CFIndex)sizeof(kReportDesc));
  CFDictionarySetValue(props, CFSTR(kIOHIDReportDescriptorKey), desc);
  CFRelease(desc);

  cf_set_int(props, CFSTR(kIOHIDVendorIDKey), 0x2E8A);
  cf_set_int(props, CFSTR(kIOHIDProductIDKey), 0x0A01);
  cf_set_int(props, CFSTR(kIOHIDPrimaryUsagePageKey), 0xFF);
  cf_set_int(props, CFSTR(kIOHIDPrimaryUsageKey), 0x01);
  CFDictionarySetValue(props, CFSTR(kIOHIDManufacturerKey), CFSTR("OAT"));
  CFDictionarySetValue(props, CFSTR(kIOHIDProductKey), CFSTR("PT1000 OAT"));
  CFDictionarySetValue(props, CFSTR(kIOHIDTransportKey), CFSTR("USB"));

  IOHIDUserDeviceRef device =
      IOHIDUserDeviceCreateWithProperties(kCFAllocatorDefault, props, 0);
  CFRelease(props);
  if (!device) {
    fprintf(stderr,
            "IOHIDUserDeviceCreateWithProperties failed.\n"
            "macOS may require entitlement com.apple.developer.hid.virtual.device.\n"
            "Use: python3 tools/emulate_oat_hid.py --tcp\n");
    return 2;
  }

  dispatch_queue_t queue =
      dispatch_queue_create("oat.hid.emu", DISPATCH_QUEUE_SERIAL);
  IOHIDUserDeviceSetDispatchQueue(device, queue);
  IOHIDUserDeviceActivate(device);

  signal(SIGINT, on_sigint);
  signal(SIGTERM, on_sigint);

  printf("HID OAT 2E8A:0A01 every %.3gs (Ctrl+C to stop)\n", interval_s);
  printf("XCSoar fork: open vendor HID 2E8A:0A01\n");
  fflush(stdout);

  const useconds_t sleep_us = (useconds_t)(interval_s * 1000000.0);
  unsigned tick = 0;
  while (g_run) {
    const double temp = base_temp + 2.0 * sin((double)tick * interval_s / 60.0);
    int32_t centi = (int32_t)lround(temp * 100.0);
    if (centi > 32767) {
      centi = 32767;
    }
    if (centi < -32768) {
      centi = -32768;
    }

    uint8_t report[4];
    report[0] = 1;
    report[1] = (uint8_t)(centi & 0xFF);
    report[2] = (uint8_t)((centi >> 8) & 0xFF);
    report[3] = 0;

    const IOReturn rc = IOHIDUserDeviceHandleReportWithTimeStamp(
        device, mach_absolute_time(), report, (CFIndex)sizeof(report));
    if (rc != kIOReturnSuccess) {
      fprintf(stderr, "HandleReport failed: 0x%x\n", rc);
    } else {
      printf("T=%.2f C  report=%02X %02X %02X %02X\n", temp, report[0],
             report[1], report[2], report[3]);
      fflush(stdout);
    }
    tick++;
    usleep(sleep_us);
  }

  IOHIDUserDeviceCancel(device);
  CFRelease(device);
  printf("\nStopped.\n");
  return 0;
}
