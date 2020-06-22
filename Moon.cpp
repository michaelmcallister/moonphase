#include "Moon.h"

#include "moonphases.h"

Moon::Moon(int day, int month, int year) {
  _day = day;
  _month = month;
  _year = year;
}

// Age calculates the age of the moon based on a 30 day lunation.
// It uses John Conways mental method documented in
// 'Winning Ways for Your Mathematical Plays' using logic broken down in
// https://www.cs.williams.edu/~bailey/cs135/lectures/Lecture02.pdf
// It only supports the 21st Century (2000-2099).
int Moon::age() {
  // Initialize the sum, s, to the sum of the day, the month, and 30. The final
  // 30 is helpful to avoid negative modular arithmetic, later.
  // We subtract 8 here for the 21st century.
  int s = _month + _day + 30 - 8;

  //  yyyy to yy.
  int yy = _year % 100;

  // Compute the distance (d) to the closest multiple of 19, but adjusted to be
  // centered around 0, i.e, -9 to 9 instead of 0 to 19.
  int d = (yy + 9) % 19 - 9;

  // to d prepend a ten's digit that is the value of |d| mod 3.
  int tens = abs(d % 3) * 10;

  // This to ensure the sign doesn't change. Surely there is a better way.
  if (d < 0) {
    d -= tens;
  } else {
    d += tens;
  }

  // Add d to s modulo 30.
  return (s + d) % 30;
}

// Phase returns the current lunar phase based on the age of the moon.
Phase Moon::phase() {
  switch (age()) {
    case 2 ... 6:
      return WAXING_CRESCENT;
    case 7 ... 8:
      return FIRST_QUARTER;
    case 9 ... 13:
      return WAXING_GIBBOUS;
    case 14 ... 16:
      return FULL;
    case 17 ... 21:
      return WANING_GIBBOUS;
    case 22 ... 23:
      return LAST_QUARTER;
    case 24 ... 28:
      return WANING_CRESCENT;
    default:
      return NEW;
  }
}

// Bitmap returns an image struct containing the bitmap data and metadata.
Moon::Image Moon::bitmap() {
  // defaults for the returned image.
  // To save space we only have one set of crescent, gibbous and quarter phases
  // and mirror them to denote waxing/waning.
  // The background and foreground colour is used for the 'new moon', when it's
  // at it's darkest. To ensure contrast, the background will be white during
  // this phase.
  Image img;
  img.mirrored = false;
  img.background = BLACK;
  img.foreground = WHITE;

  switch (phase()) {
    case WANING_CRESCENT:
      img.mirrored = true;
    case WAXING_CRESCENT:
      img.data = crescent;
      img.width = 121;
      img.height = 195;
      break;
    case FIRST_QUARTER:
      img.mirrored = true;
    case LAST_QUARTER:
      img.data = quarter;
      img.width = 138;
      img.height = 197;
      break;
    case WANING_GIBBOUS:
      img.mirrored = true;
    case WAXING_GIBBOUS:
      img.data = gibbous;
      img.width = 158;
      img.height = 195;
      break;
    case FULL:
      img.data = full;
      img.height = 198;
      img.width = 198;
      break;
    case NEW:
      img.data = full;
      img.height = 198;
      img.width = 198;
      img.background = WHITE;
      img.foreground = BLACK;
  }
  return img;
}
