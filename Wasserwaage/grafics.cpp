/* grafics.cpp    Womo Bilder für TFT
 * 
 */

#include "grafics.h"

// extern fontdatatype MediumNumbers[] PROGMEM =
// extern fontdatatype BigNumbers[] PROGMEM =

const uint8_t Auto2_00[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x00, 0x3f, 0xff, 0xff, 
  0xe7, 0xff, 0xdf, 0xff, 0xff, 
  0xdf, 0xff, 0xe0, 0x00, 0x01, 
  0xdf, 0xff, 0xff, 0xff, 0xfe, 
  0xdf, 0xff, 0xff, 0xff, 0xfe, 
  0xdf, 0xff, 0xff, 0xff, 0xfe, 
  0xe0, 0x3f, 0xff, 0xff, 0xfe, 
  0xff, 0xbf, 0xff, 0xff, 0xfe, 
  0xff, 0xbf, 0xff, 0xff, 0xfe, 
  0xff, 0x7f, 0xff, 0xff, 0xfe, 
  0xff, 0x7f, 0xff, 0xff, 0xfe, 
  0xfe, 0xff, 0xff, 0xff, 0xfe, 
  0xfe, 0xff, 0xff, 0xff, 0xfe, 
  0xfd, 0xff, 0xff, 0xff, 0xfe, 
  0xc1, 0xff, 0xff, 0xff, 0xfe, 
  0xbf, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0x1f, 0xff, 0xf8, 0xfe, 
  0x7e, 0xef, 0xff, 0xf7, 0x7e, 
  0x80, 0xe0, 0x00, 0x07, 0x01, 
  0xfe, 0xef, 0xff, 0xf7, 0x7f, 
  0xff, 0x1f, 0xff, 0xf8, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_05[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfc, 0x01, 0xff, 0xff, 0xff, 
  0xf3, 0xfe, 0x3f, 0xff, 0xff, 
  0xef, 0xff, 0xcf, 0xff, 0xff, 
  0xef, 0xff, 0xf0, 0x7f, 0xff, 
  0xef, 0xff, 0xff, 0x80, 0x0f, 
  0xe7, 0xff, 0xff, 0xff, 0xf1, 
  0xf0, 0x1f, 0xff, 0xff, 0xfe, 
  0xff, 0xdf, 0xff, 0xff, 0xfe, 
  0xff, 0xdf, 0xff, 0xff, 0xfe, 
  0xff, 0xbf, 0xff, 0xff, 0xfe, 
  0xff, 0xbf, 0xff, 0xff, 0xfe, 
  0xff, 0x7f, 0xff, 0xff, 0xfe, 
  0xfe, 0xff, 0xff, 0xff, 0xfe, 
  0xfd, 0xff, 0xff, 0xff, 0xfe, 
  0xc1, 0xff, 0xff, 0xff, 0xfd, 
  0xbf, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0x7e, 0x3f, 0xff, 0xff, 0xfb, 
  0x9d, 0xdf, 0xff, 0xff, 0xfb, 
  0xe1, 0xc1, 0xff, 0xf1, 0xfb, 
  0xfd, 0xde, 0x07, 0xee, 0xfb, 
  0xfe, 0x3f, 0xf8, 0x0e, 0x1b, 
  0xff, 0xff, 0xff, 0xee, 0xe7, 
  0xff, 0xff, 0xff, 0xf1, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_10[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x03, 0xff, 0xff, 0xff, 
  0xfd, 0xfc, 0x7f, 0xff, 0xff, 
  0xfb, 0xff, 0x9f, 0xff, 0xff, 
  0xfb, 0xff, 0xe7, 0xff, 0xff, 
  0xfb, 0xff, 0xfb, 0xff, 0xff, 
  0xfb, 0xff, 0xfc, 0x0f, 0xff, 
  0xfc, 0x1f, 0xff, 0xf0, 0xff, 
  0xff, 0xef, 0xff, 0xff, 0x03, 
  0xff, 0xef, 0xff, 0xff, 0xfd, 
  0xff, 0xdf, 0xff, 0xff, 0xfe, 
  0xff, 0xbf, 0xff, 0xff, 0xfe, 
  0xff, 0x7f, 0xff, 0xff, 0xfe, 
  0xfe, 0xff, 0xff, 0xff, 0xfe, 
  0xe0, 0xff, 0xff, 0xff, 0xfe, 
  0xdf, 0xff, 0xff, 0xff, 0xfd, 
  0xbf, 0xff, 0xff, 0xff, 0xfd, 
  0xbf, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfd, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0x7e, 0x3f, 0xff, 0xff, 0xfb, 
  0x8d, 0xdf, 0xff, 0xff, 0xfb, 
  0xf1, 0xcf, 0xff, 0xff, 0xfb, 
  0xfd, 0xd0, 0x7f, 0xe3, 0xf7, 
  0xfe, 0x3f, 0x81, 0xdd, 0xf7, 
  0xff, 0xff, 0xfe, 0x1c, 0xf7, 
  0xff, 0xff, 0xff, 0xdd, 0x0f, 
  0xff, 0xff, 0xff, 0xe3, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_0_05[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x00, 0xff, 0xff, 0xff, 
  0xe7, 0xff, 0x7f, 0xfc, 0x07, 
  0xdf, 0xff, 0xbc, 0x03, 0xfb, 
  0xbf, 0xff, 0xc3, 0xff, 0xfb, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0xbf, 0xff, 0xff, 0xff, 0xfd, 
  0xb9, 0xff, 0xff, 0xff, 0xfd, 
  0xc6, 0xff, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xfe, 0xff, 0xff, 0xff, 0xfd, 
  0xfd, 0xff, 0xff, 0xff, 0xfe, 
  0xfb, 0xff, 0xff, 0xff, 0xfe, 
  0xe7, 0xff, 0xff, 0xff, 0xfe, 
  0xdf, 0xff, 0xff, 0xff, 0xfe, 
  0x3f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xff, 0xfe, 
  0x7f, 0xff, 0xff, 0xf8, 0xfe, 
  0x7f, 0xff, 0xff, 0xf7, 0x7d, 
  0x7f, 0x1f, 0xff, 0xe7, 0x03, 
  0x7e, 0xef, 0x80, 0x17, 0x7f, 
  0x70, 0xe0, 0x7f, 0xf8, 0xff, 
  0x8e, 0xef, 0xff, 0xff, 0xff, 
  0xff, 0x1f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_0_10[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xcf, 
  0xfc, 0x1f, 0xff, 0xf8, 0x37, 
  0xf3, 0xe3, 0xff, 0x07, 0xf7, 
  0xef, 0xfc, 0x78, 0xff, 0xfb, 
  0xdf, 0xff, 0x87, 0xff, 0xfb, 
  0xbf, 0xff, 0xff, 0xff, 0xfb, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0x7f, 0xff, 0xff, 0xff, 0xfb, 
  0x79, 0xff, 0xff, 0xff, 0xfb, 
  0x86, 0xff, 0xff, 0xff, 0xfd, 
  0xfe, 0xff, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xff, 0x7f, 0xff, 0xff, 0xfd, 
  0xfe, 0xff, 0xff, 0xff, 0xfe, 
  0xfd, 0xff, 0xff, 0xff, 0xfe, 
  0xf3, 0xff, 0xff, 0xff, 0xfe, 
  0xef, 0xff, 0xff, 0xff, 0xfe, 
  0x9f, 0xff, 0xff, 0xfc, 0x7e, 
  0x7f, 0xff, 0xff, 0xfb, 0xb1, 
  0x7f, 0xff, 0xff, 0xf3, 0x8f, 
  0x7f, 0xff, 0xff, 0x8b, 0xbf, 
  0x7f, 0x8f, 0xf0, 0x7c, 0x7f, 
  0xbf, 0x74, 0x0f, 0xff, 0xff, 
  0xbe, 0x73, 0xff, 0xff, 0xff, 
  0xc1, 0x77, 0xff, 0xff, 0xff, 
  0xff, 0x8f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto3_00[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xff, 0xaf, 0xfa, 0xff, 
  0xff, 0xaf, 0xfa, 0xff, 
  0xff, 0xaf, 0xfa, 0xff, 
  0xff, 0x8f, 0xf8, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto3_05[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x7f, 0xff, 
  0xfe, 0xff, 0x80, 0x3f, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0x00, 0xff, 0xbf, 
  0xfe, 0xff, 0x00, 0x3f, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfb, 0xff, 0xff, 0x7f, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xfc, 0x00, 0xfe, 0xff, 
  0xfe, 0xbf, 0x00, 0xff, 
  0xfe, 0xbf, 0xeb, 0xff, 
  0xfe, 0xbf, 0xeb, 0xff, 
  0xfe, 0x3f, 0xeb, 0xff, 
  0xff, 0xff, 0xe3, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto3_10[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0x83, 0xff, 0xff, 
  0xff, 0x7c, 0x1f, 0xff, 
  0xff, 0x7f, 0xe0, 0x3f, 
  0xff, 0x7f, 0xff, 0xdf, 
  0xff, 0x03, 0xff, 0xdf, 
  0xfe, 0xfc, 0x0f, 0xdf, 
  0xfe, 0xff, 0xf0, 0x3f, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfd, 0xff, 0xff, 0xbf, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xfd, 0xff, 0xff, 0x7f, 
  0xf9, 0xff, 0xff, 0x7f, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xfb, 0xff, 0xfe, 0xff, 
  0xf7, 0xff, 0xfe, 0xff, 
  0xf7, 0xff, 0xfe, 0xff, 
  0xf8, 0x0f, 0xfe, 0xff, 
  0xfe, 0xb0, 0x3e, 0xff, 
  0xfe, 0xbf, 0x81, 0xff, 
  0xfe, 0xbf, 0xaf, 0xff, 
  0xfe, 0x3f, 0xaf, 0xff, 
  0xff, 0xff, 0xaf, 0xff, 
  0xff, 0xff, 0x8f, 0xff, 
};

// ============================================
// volle Fahrzeuge

const uint8_t Auto2_00f[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x00, 0x3f, 0xff, 0xff, 
  0xe0, 0x00, 0x1f, 0xff, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x01, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0x80, 0x00, 0x00, 0x00, 
  0xff, 0x80, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xe0, 0x00, 0x07, 0x00, 
  0x80, 0xe0, 0x00, 0x07, 0x01, 
  0xfe, 0xef, 0xff, 0xf7, 0x7f, 
  0xff, 0x1f, 0xff, 0xf8, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_05f[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfc, 0x01, 0xff, 0xff, 0xff, 
  0xf0, 0x00, 0x3f, 0xff, 0xff, 
  0xe0, 0x00, 0x0f, 0xff, 0xff, 
  0xe0, 0x00, 0x00, 0x7f, 0xff, 
  0xe0, 0x00, 0x00, 0x00, 0x0f, 
  0xe0, 0x00, 0x00, 0x00, 0x01, 
  0xf0, 0x00, 0x00, 0x00, 0x00, 
  0xff, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0x80, 0x00, 0x00, 0x00, 
  0xff, 0x80, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0x00, 0x00, 0x00, 0x01, 
  0x80, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x81, 0xc0, 0x00, 0x00, 0x03, 
  0xe1, 0xc0, 0x00, 0x00, 0x03, 
  0xfd, 0xde, 0x00, 0x0e, 0x03, 
  0xfe, 0x3f, 0xf8, 0x0e, 0x03, 
  0xff, 0xff, 0xff, 0xee, 0xe7, 
  0xff, 0xff, 0xff, 0xf1, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_10f[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x03, 0xff, 0xff, 0xff, 
  0xfc, 0x00, 0x7f, 0xff, 0xff, 
  0xf8, 0x00, 0x1f, 0xff, 0xff, 
  0xf8, 0x00, 0x07, 0xff, 0xff, 
  0xf8, 0x00, 0x03, 0xff, 0xff, 
  0xf8, 0x00, 0x00, 0x0f, 0xff, 
  0xfc, 0x00, 0x00, 0x00, 0xff, 
  0xff, 0xe0, 0x00, 0x00, 0x03, 
  0xff, 0xe0, 0x00, 0x00, 0x01, 
  0xff, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0x80, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0x00, 0x00, 0x00, 0x01, 
  0x80, 0x00, 0x00, 0x00, 0x01, 
  0x80, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x81, 0xc0, 0x00, 0x00, 0x03, 
  0xf1, 0xc0, 0x00, 0x00, 0x03, 
  0xfd, 0xd0, 0x00, 0x00, 0x07, 
  0xfe, 0x3f, 0x80, 0x1c, 0x07, 
  0xff, 0xff, 0xfe, 0x1c, 0x07, 
  0xff, 0xff, 0xff, 0xdd, 0x0f, 
  0xff, 0xff, 0xff, 0xe3, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_0_05f[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xf8, 0x00, 0xff, 0xff, 0xff, 
  0xe0, 0x00, 0x7f, 0xfc, 0x07, 
  0xc0, 0x00, 0x3c, 0x00, 0x03, 
  0x80, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x80, 0x00, 0x00, 0x00, 0x01, 
  0x80, 0x00, 0x00, 0x00, 0x01, 
  0xc0, 0x00, 0x00, 0x00, 0x01, 
  0xe3, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xfe, 0x00, 0x00, 0x00, 0x01, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 
  0xf8, 0x00, 0x00, 0x00, 0x00, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0x01, 
  0x00, 0x00, 0x00, 0x07, 0x03, 
  0x00, 0xe0, 0x00, 0x17, 0x7f, 
  0x00, 0xe0, 0x7f, 0xf8, 0xff, 
  0x8e, 0xef, 0xff, 0xff, 0xff, 
  0xff, 0x1f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto2_0_10f[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xcf, 
  0xfc, 0x1f, 0xff, 0xf8, 0x07, 
  0xf0, 0x03, 0xff, 0x00, 0x07, 
  0xe0, 0x00, 0x78, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x03, 
  0x80, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x03, 
  0x86, 0x00, 0x00, 0x00, 0x01, 
  0xfe, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xff, 0x00, 0x00, 0x00, 0x01, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 
  0xfc, 0x00, 0x00, 0x00, 0x00, 
  0xf0, 0x00, 0x00, 0x00, 0x00, 
  0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x81, 
  0x00, 0x00, 0x00, 0x03, 0x8f, 
  0x00, 0x00, 0x00, 0x0b, 0xbf, 
  0x00, 0x00, 0x00, 0x7c, 0x7f, 
  0x80, 0x70, 0x0f, 0xff, 0xff, 
  0x80, 0x73, 0xff, 0xff, 0xff, 
  0xc1, 0x77, 0xff, 0xff, 0xff, 
  0xff, 0x8f, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto3_00f[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0xff, 0xff, 0xbf, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xff, 0x8f, 0xf8, 0xff, 
  0xff, 0x8f, 0xf8, 0xff, 
  0xff, 0x8f, 0xf8, 0xff, 
  0xff, 0x8f, 0xf8, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto3_05f[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 
  0xfe, 0x00, 0x7f, 0xff, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0xff, 0x80, 0x3f, 
  0xfe, 0x00, 0x7f, 0xbf, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xf8, 0x00, 0x00, 0x7f, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xfc, 0x00, 0x00, 0xff, 
  0xfe, 0x3f, 0x00, 0xff, 
  0xfe, 0x3f, 0xe3, 0xff, 
  0xfe, 0x3f, 0xe3, 0xff, 
  0xfe, 0x3f, 0xe3, 0xff, 
  0xff, 0xff, 0xe3, 0xff, 
  0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto3_10f[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0x83, 0xff, 0xff, 
  0xff, 0x00, 0x1f, 0xff, 
  0xff, 0x00, 0x00, 0x3f, 
  0xff, 0x00, 0x00, 0x1f, 
  0xff, 0x78, 0x00, 0x1f, 
  0xfe, 0x07, 0xf8, 0x1f, 
  0xfe, 0x00, 0x07, 0xbf, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfc, 0x00, 0x00, 0x3f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xfc, 0x00, 0x00, 0x7f, 
  0xf8, 0x00, 0x00, 0x7f, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf0, 0x00, 0x00, 0xff, 
  0xf0, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xfe, 0x30, 0x00, 0xff, 
  0xfe, 0x3f, 0x81, 0xff, 
  0xfe, 0x3f, 0x8f, 0xff, 
  0xfe, 0x3f, 0x8f, 0xff, 
  0xff, 0xff, 0x8f, 0xff, 
  0xff, 0xff, 0x8f, 0xff, 
};

// händisch schlecht
const uint8_t Auto3_10f__[] PROGMEM= {
  0xff, 0xff, 0xff, 0xff, 
  0xff, 0x83, 0xff, 0xff, 
  0xff, 0x00, 0x1f, 0xff, 
  0xff, 0x00, 0x00, 0x3f, 
  0xff, 0x00, 0x00, 0x1f, 
  0xff, 0x00, 0x00, 0x1f, 
  0xfe, 0x00, 0x00, 0x1f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0x00, 0x00, 0x3f, 
  0xfe, 0xff, 0xff, 0x3f, 
  0xfd, 0x00, 0x00, 0x3f, 
  0xfd, 0x00, 0x00, 0x7f, 
  0xfd, 0x00, 0x00, 0x7f, 
  0xfd, 0x00, 0x00, 0x7f, 
  0xfd, 0x00, 0x00, 0x7f, 
  0xf8, 0x00, 0x00, 0x7f, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xf0, 0x00, 0x00, 0xff, 
  0xf0, 0x00, 0x00, 0xff, 
  0xf8, 0x00, 0x00, 0xff, 
  0xfe, 0x00, 0x00, 0xff, 
  0xfe, 0x00, 0x00, 0xff, 
  0xfe, 0x00, 0x0f, 0xff, 
  0xfe, 0x00, 0x0f, 0xff, 
  0xff, 0xff, 0x0f, 0xff, 
  0xff, 0xff, 0x8f, 0xff, 
};

/*
//01 - altes Auto
const uint8_t Auto01[] PROGMEM = {  //Pixel set is white
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0x00, 0x00, 0x03, 
  0xff, 0xf7, 0xff, 0xff, 0xfb, 
  0xff, 0xf7, 0xff, 0xff, 0xfb, 
  0xff, 0xef, 0xff, 0xff, 0xfb, 
  0xff, 0xef, 0xff, 0xff, 0xfb, 
  0xff, 0xef, 0xff, 0xff, 0xfb, 
  0xff, 0xef, 0xff, 0xff, 0xfb, 
  0xff, 0xef, 0xff, 0xff, 0xfb, 
  0xff, 0xdf, 0xff, 0xff, 0xfb, 
  0xff, 0xdf, 0xff, 0xff, 0xfb, 
  0xf0, 0x1f, 0xff, 0xff, 0xfb, 
  0xf7, 0xff, 0xff, 0xff, 0xfb, 
  0xf7, 0xff, 0xff, 0xff, 0xfb, 
  0xf7, 0xff, 0xff, 0xff, 0xfb, 
  0xf7, 0xc7, 0xff, 0xff, 0xfb, 
  0xf7, 0xbb, 0xff, 0xfc, 0x3b, 
  0xf0, 0x38, 0x00, 0x03, 0xc3, 
  0xff, 0xc7, 0xff, 0xfb, 0xdf, 
  0xff, 0xff, 0xff, 0xfc, 0x3f, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto05[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf3, 0xff, 0xff, 0xff, 
  0xff, 0xf0, 0x00, 0x3f, 0xff, 
  0xff, 0xf7, 0xfe, 0x00, 0x07, 
  0xff, 0xef, 0xff, 0xff, 0xc3, 
  0xff, 0xcf, 0xff, 0xff, 0xfb, 
  0xff, 0xcf, 0xff, 0xff, 0xf3, 
  0xff, 0xcf, 0xff, 0xff, 0xf3, 
  0xff, 0xdf, 0xff, 0xff, 0xf3, 
  0xff, 0xbf, 0xff, 0xff, 0xf7, 
  0xff, 0xbf, 0xff, 0xff, 0xf7, 
  0xe0, 0x3f, 0xff, 0xff, 0xf7, 
  0xef, 0xff, 0xff, 0xff, 0xf7, 
  0xcf, 0xff, 0xff, 0xff, 0xf7, 
  0xcf, 0xff, 0xff, 0xff, 0xe7, 
  0xcf, 0x1f, 0xff, 0xff, 0xe7, 
  0xce, 0x6f, 0xff, 0xff, 0xe7, 
  0xc0, 0xe3, 0xff, 0xff, 0xe7, 
  0xff, 0x00, 0x00, 0x70, 0xef, 
  0xff, 0xff, 0xfc, 0x0f, 0x0f, 
  0xff, 0xff, 0xff, 0xef, 0x0f, 
  0xff, 0xff, 0xff, 0xf0, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
};

const uint8_t Auto10[] PROGMEM = {  
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xf8, 0x7f, 0xff, 0xff, 
  0xff, 0xf3, 0x01, 0xff, 0xff, 
  0xff, 0xf3, 0xfc, 0x07, 0xff, 
  0xff, 0xef, 0xff, 0xe0, 0x3f, 
  0xff, 0xef, 0xff, 0xff, 0x83, 
  0xff, 0xef, 0xff, 0xff, 0xf9, 
  0xff, 0xcf, 0xff, 0xff, 0xfb, 
  0xff, 0xdf, 0xff, 0xff, 0xfb, 
  0xe7, 0xbf, 0xff, 0xff, 0xf3, 
  0xe0, 0x3f, 0xff, 0xff, 0xf3, 
  0xef, 0xff, 0xff, 0xff, 0xf7, 
  0xcf, 0xff, 0xff, 0xff, 0xf7, 
  0xcf, 0xff, 0xff, 0xff, 0xf7, 
  0xdf, 0x3f, 0xff, 0xff, 0xf7, 
  0xc6, 0x0f, 0xff, 0xff, 0xf7, 
  0xe0, 0xef, 0xff, 0xff, 0xe7, 
  0xff, 0x00, 0xff, 0xff, 0xe7, 
  0xff, 0x9c, 0x03, 0xff, 0xef, 
  0xff, 0xff, 0xf0, 0x00, 0xef, 
  0xff, 0xff, 0xff, 0xce, 0x4f, 
  0xff, 0xff, 0xff, 0xee, 0x0f, 
  0xff, 0xff, 0xff, 0xe0, 0xff, 
};
*/