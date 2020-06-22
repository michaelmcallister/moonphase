
#include <GxEPD2_3C.h>

#include "Moon.h"

#define MAX_DISPLAY_BUFFER_SIZE 800
#define MAX_HEIGHT_3C(EPD)                                         \
  (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) \
       ? EPD::HEIGHT                                               \
       : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))

GxEPD2_3C<GxEPD2_154c, MAX_HEIGHT_3C(GxEPD2_154c)> display(
    GxEPD2_154c(/*CS=10*/ SS, /*DC=*/8, /*RST=*/9, /*BUSY=*/7));

void draw(Moon::Image i);

void setup() {
  display.init(115200);
  Moon m = Moon(5, 7, 2020);
  draw(m.bitmap());
}

void loop() {}

void draw(Moon::Image i) {
  display.setFullWindow();
  display.firstPage();
  display.mirror(i.mirrored);
  do {
    display.fillScreen(i.background);
    display.drawBitmap(0, 0, i.data, i.width, i.height, i.foreground);
  } while (display.nextPage());
}
