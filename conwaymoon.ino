
#include <Adafruit_SleepyDog.h>
#include <GxEPD2_3C.h>
#include <RTClib.h>
#include <Wire.h>

#include "Moon.h"

#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT_3C(EPD)                                         \
  (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) \
       ? EPD::HEIGHT                                               \
       : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))

GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(
    GxEPD2_154c(/*CS=10*/ SS, /*DC=*/8, /*RST=*/9, /*BUSY=*/7));

RTC_DS1307 RTC;

void draw(Moon m);

// These globals track state to ensure the screen is only updated when needed.
int last_known_day = 0;
Phase last_known_phase = UNKNOWN;

void setup() {
  display.init(115200);
  Wire.begin();
  RTC.begin();

  // The box is on it's side, USB port on the left hand side, with the screen
  // mounted sideways.
  display.setRotation(3);

  pinMode(LED_BUILTIN, OUTPUT);

  // Check to see if the RTC is keeping time. If not use the compiled time.
  if (!RTC.isrunning()) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  DateTime now = RTC.now();
  if (now.day() != last_known_day) {
    Moon m = Moon(now.day(), now.month(), now.year());
    // Only bother re-drawing when both the day AND the phase is different.
    if (m.phase() != last_known_phase) {
      draw(m);
      last_known_phase = m.phase();
    }
    last_known_day = now.day();
  }

  // Zzzz
  display.powerOff();
  Watchdog.sleep();
}

void draw(Moon m) {
  Moon::Image i = m.bitmap();
  display.setFullWindow();
  display.firstPage();
  do {
    // Mirroring has to happen before any bitmaps are drawn.
    display.mirror(i.mirrored);
    display.fillScreen(i.background);
    display.drawBitmap(0, 0, i.data, i.width, i.height, i.foreground);
    switch (m.phase()) {
      // Quarter phases use the full moon bitmap with a square drawn over it.
      case FIRST_QUARTER:
        display.fillRect(99, 0, i.width / 2, 200, i.background);
        break;
      case LAST_QUARTER:
        display.fillRect(0, 0, i.width / 2, 200, i.background);
        break;
    }
  } while (display.nextPage());
}
