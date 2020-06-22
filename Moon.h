/*
  Moon.h - Library for calculating the lunar phase and retrieving a bitmap based
    on current phase.
  Created by Michael McAllister, 19 June, 2020.
*/
#ifndef Moon_h
#define Moon_h

#define WHITE 0xFFFF
#define BLACK 0x0000

enum Phase {
  NEW,
  WAXING_CRESCENT,
  FIRST_QUARTER,
  WAXING_GIBBOUS,
  FULL,
  WANING_GIBBOUS,
  LAST_QUARTER,
  WANING_CRESCENT
};

class Moon {
 public:
  struct Image {
    int height;
    int width;
    bool mirrored;
    int background;
    int foreground;
    const unsigned char* data;
  };
  Moon(int day, int month, int year);
  int age();
  Phase phase();
  Image bitmap();

 private:
  int _day;
  int _month;
  int _year;
};

#endif