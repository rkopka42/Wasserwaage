/* defines.h
 * 
 */

#ifndef _DEFINES_H
#define _DEFINES_H

#define VERSION 9006  // Format v.ss.xxx wie in der Firma - keine führende 0!!
#define VERSIONSTRING "0.09.006"  // automatisch generieren ?

#define CFGFILE_NAME "/Wasserwaage.cfg"
#define LOGFILE_NAME "/Wasserwaage.log"

#define DSERIAL 1    // dann gibts Ausgaben - sind im fertigen System nicht nötig, für größere Blöcke, sonst DSerial

#define DSerial  if (DSERIAL) Serial

//#define DO_SLEEP  // Prozessor Sleep durchführen
#define DO_DEBUG  // show Debugdata on serial

#define USE_M5STACK   // nur dann die entsprechenden Sensoren erlauben, bzw. wählen, auch die includes

//#define USE_KOMPASS   // externer Kompaßsensor
#define USE_M5KOMPASS   // den vom M5 Modul
#define SHOW_KOMPASS    // nur für die Anzeige

#define USE_TEMP      // externer Temperatursensor oder vom M5 Modul
#define USE_DISPLAY   // evt. später noch Unterscheidung nach TFT von letzter Version oder M5Stack, erstmal ohne M5 den alten Code nehmen

// manche Libs geben tft fix vor. Für unterschiedliche Libs daher immer tft_ im Source und mit #define auf das richtige umsetzen
#ifdef USE_M5STACK
 #define tft_ M5.lcd
 #ifdef USE_DISPLAY
  #define LCD
 #endif
#endif

// Pins
#ifndef LED_BUILTIN
 #define LED_BUILTIN 2    // dummy
#endif

//#define RXD2 16   // Serial2  ESP32
//#define TXD2 17
#define TASTER 32 // AP/WLAN Umschaltung

#define ANALOGINPIN 34  // Spannung ? -> define  ADC1_6
// -> D+ ?? aber auch VCC einlesen, D+ dann nur zum Schalten

// externes TFT Display, M5Stack macht das intern
#define TFT_CS        10
#define TFT_RST        8 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         9

#ifdef USE_M5STACK
 #define CAL_KEY 37  // ButtonC M5STACK für Kalibrierung Mitte ?
 #define LICHT   15  // LED, die den Zustand zeigt: Standby, connected...  GPIO15 für M5Stack ???
#else
 #define LICHT   13  // LED, die den Zustand zeigt: Standby, connected...  
#endif

#define MODE_NOCLIENT 1 //  D+ aber kein Client, WLAN bleibt an
#define MODE_SLOW     2 //  D+ und Client
#define MODE_FAST     3 //  D+ und Client und Anwahl in der GUI (10min lang, dann SLOW)
#define MODE_NODPLUS  4 //  kein D+, aber vorher Client, SLOW, nach 10min in SLEEP 
#define MODE_WLAN     5 //  kein D+, durch Taste (wie geht das mit längerem Sleep ? Zeiten für pressed_long anschauen)  AP ein für 10min oder bis wieder Taste. Dann SLEEP
#define MODE_SLEEP    0 //  länger Sleep, sonst nichts, auf Taste und D+ achten, dann NOCLIENT oder WLAN
#define MODE_NODPLUSFAST  6 //  kein D+, aber vorher Client (FAST), SLOW, nach 10min in SLEEP 

#define MODETIMEOUT 30 //600 // sek bis ein Mode Wechsel auftritt

// Werte, die man konfigurieren könnte, aber nur für Feeling, erstmal nur im Source lassen
#define KEYTIME_MIN   100   // ms
#define KEYTIME_SHORT 500
#define KEYTIME_LONG  2000
#define SUMCNT 16           // Mittelung - neuer Wert mit 1/16 gewichtet
#define CALC_PER_DISPLAY 32 // 16x berechnen (und mitteln) vor Änderung der Anzeige
#define MAX_SHOW 99         // max. Zahlenwert für die Höhe in cm
#define MAX_VALUE 20        // cm Höhe für max. Höhe des Balkens
#define KEIL_HOEHE 10       // cm Höhe für Linie für Keilhöhe
#define WIDTH_BALKEN_H 30   // Werte für html
#define HEIGHT_BALKEN_H 160 // Balkenhöhe am Display

#define HEIGHT_OK 3         // unter 3cm wird OK angezeigt
#define DREIECK_KANTE 7     // Kantenlänge des Dreiecks bei den Balkenanzeigen in Pixel
#define WINKEL_ROT  3       // ab dem Winkel in Deg wird das Bild rot angezeigt
#define WINKEL_GELB 1       // .. gelb

// Pins
#ifndef USE_M5STACK

 //#define ST77XX_BLACK BLACK   // ????
 //#define ST77XX_WHITE WHITE

 #define BLACK ST77XX_BLACK   // für TFT Display nötig
 #define WHITE ST77XX_WHITE 
 
#endif

// Zeiten für Signal LED
#define LED_ON  30    // ms
#define LED_OFF 2970  
#define LED_ON_OK  150  // ms
#define LED_OFF_OK 150  

// TFT
#define GRUEN 0
#define ROT   1
#define GELB  2
#define BLAU  3

#define ST77XX_BLUE BLUE
#define ST77XX_BLACK BLACK
#define ST77XX_WHITE WHITE
#define ST77XX_GREEN GREEN
#define ST77XX_RED RED
#define ST77XX_YELLOW YELLOW

// Werte für Displays x,y,w,h
#ifdef USE_M5STACK
// 320x240

#define WIDTH_BALKEN 15
#define HEIGHT_BALKEN 75    // Balkenhöhe am Display -> nicht ändern

#define TEXTHALBLINKS 32
#define TEXTMITTEOBENX 140
#define TEXTRECHTSX 275 //132
#define TEXTUNTENY 213
#define VALUEOBENY 105  // 102
#define VALUEUNTENY 205 // war 205
#define VALUERECHTSX 293 //169 

#define BITMAPOBENX 120 //50    // Faktor 2 2
#define BITMAPOBENY 21
#define VECTOROBENX 155 //50    // Faktor 2 2
#define VECTOROBENY 65

#define BITMAPUNTENX 132 //62   // Faktor 2 2
#define BITMAPUNTENY 118 
#define VECTORUNTENX 160 //62   // Faktor 2 2
#define VECTORUNTENY 165 

#define BITMAPOBENW 40
#define BITMAPOBENH 30
#define BITMAPUNTENW 32
#define BITMAPUNTENH 30 
#define ANGLEX 131 // 61
#define ANGLEOBENY 108//81
#define ANGLEUNTENY 213 //194

#define FAKTOR_OBEN 0.9
#define FAKTOR_UNTEN 0.9

#else
// 128x160 TFT 1,8"

#define WIDTH_BALKEN 10
#define HEIGHT_BALKEN 50    // Balkenhöhe am Display -> nicht ändern

#define TEXTMITTEOBENX 54
#define TEXTRECHTSX 88
#define TEXTUNTENY 142
#define VALUEOBENY 68
#define VALUEUNTENY 137
#define VALUERECHTSX 113 
#define BITMAPOBENX 45
#define BITMAPOBENY 20
#define BITMAPUNTENX 52
#define BITMAPUNTENY 80 
#define BITMAPOBENW 40
#define BITMAPOBENH 30
#define BITMAPUNTENW 32
#define BITMAPUNTENH 30 
#define ANGLEX 41
#define ANGLEOBENY 54
#define ANGLEUNTENY 116
#endif

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

// ??????????  F() ignorieren für ESP
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define FILE_WRITE  "w"
#define FILE_READ   "r"
#define FILE_APPEND "a"

typedef struct {
  float angle;   // Rotation um Z Achse UZS  -> unklar ob wirklich alles abgedeckt, aber grundsätzlich sieht es gut aus
  float laenge;  // muß kein float sein, aber Berechnungen sind immer in float(==double)
  float breite1;
  float breite2; // Hinterachse hat mehr Spurbreite(?)
  float fcp;     // Kalibrierwerte -> Hidden ?
  float fcr;
  int   achse0;  // Index gibt Achsnutzung an xyz   Wert gibt Achse des Sensors an 1 ist die x Achse nach Platinenaufdruck
  int   achse1;
  int   achse2;
                      // Vorzeichen drehen die Richtung um, Index nicht ab 0, da dort kein Vorzeichen geht !
  int    maxX;     // Grenzwerte der Kompaßwerte aus der Kalibrierung
  int    minX;
  int    maxY;
  int    minY;                      
  float  angle2;   // Rotation um Z Achse UZS  -> unklar ob wirklich alles abgedeckt, aber grundsätzlich sieht es gut aus  
     
  String ssid;    // oder char[] ?
  String password;
  String host;    // unklar
}  confvalues_t;

extern int   nb; // number of clients
extern float fp_corr,fr_corr;
extern float z1,z2,z3,z4;
extern bool  do_calib;  // compass
extern float keilhoehe;
extern float fp,fr;    // auch von Calib (http) benutzt

void get_angle(char *buf, float value);
void get_height(char *buf_, int val, int max_val, bool left);
#endif
