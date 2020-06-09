/* defines.h
 * 
 */

#define VERSION 9002  // Format v.ss.xxx wie in der Firma - keine führende 0!!
#define VERSIONSTRING "0.09.002"  // automatisch generieren ?

#define CFGFILE_NAME "/Wasserwaage.cfg"
#define LOGFILE_NAME "/Wasserwaage.log"

#define SEROUT    // dann gibts Ausgaben - sind im fertigen System nicht nötig, für größere Blöcke, sonst DSerial
// wie oben, aber mit Wert für einfachen Austausch, könnte man auch noch auf Variable ändern für dynamische Anpassungen
#ifdef SEROUT
  #define DSERIAL 1
#else
  #define DSERIAL 0
#endif  
// ? define für Serial.print ...   Makros mit . gehen nicht (?)
#define DSerial  if (DSERIAL) Serial

#define DO_SLEEP  // use Sleeping mode
#define DO_DEBUG  // show Debugdata on serial

//#define USE_M5STACK   // nur dann die entsprechenden Sensoren erlauben, bzw. wählen, auch die includes
#define USE_KOMPASS   // externer Kompaßsensor
#define SHOW_KOMPASS    // nur für die Anzeige
//#define USE_M5KOMPASS // den vom M5 Modul
#define USE_TEMP      // externer Temperatursensor
//#define USE_DISPLAY   // evt. später noch Unterscheidung nach TFT von letzter Version oder M5Stack, erstmal ohne M5 den alten Code

#ifdef USE_M5STACK
 #define tft_ M5.lcd
 #ifdef USE_DISPLAY
  #define LCD
 #endif
 #define LED_BUILTIN 2    // dummy
#endif

// -> evt. das Ini System vom Wasserstop

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
#define DREIECK_KANTE 7

// Config defaults
#define DEF_ANGLE     0   // Grad Drehung des Sensors
#define DEF_LAENGE  381 // für Test 380   // cm
#define DEF_BREITE1 184
#define DEF_BREITE2 202   // ???
#define WINKEL_ROT 3
#define WINKEL_GELB 1

// Pins
#ifdef USE_M5STACK
 #define LICHT   15  // LED, die den Zustand zeigt: Standby, connected...  GPIO15 für M5Stack ???
#else
 #define LICHT   13  // LED, die den Zustand zeigt: Standby, connected... 
#endif
#define CAL_KEY 37  // ButtonC
#define RXD2 16   // Serial2  ESP32  oder SWSerial ?
#define TXD2 17

// Zeiten für Signal LED
#define LED_ON  30    // ms
#define LED_OFF 2970  
#define LED_ON_OK  150  // ms
#define LED_OFF_OK 150  

//#define EEPROM_SIZE 1024

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

// Serial2
#define SEARCH 0
#define START 1
#define SEARCHCMD 2
#define END 3
#define READY 4

// Werte für Displays x,y,w,h
#ifdef USE_M5STACK
// 320x240
#define WIDTH_BALKEN 15
#define HEIGHT_BALKEN 75    // Balkenhöhe am Display -> nicht ändern

#define TEXTMITTEOBENX 80
#define TEXTRECHTSX 132
#define TEXTUNTENY 213
#define VALUEOBENY 102
#define VALUEUNTENY 205
#define VALUERECHTSX 169 

//#define BITMAPOBENX 67  // Faktor 1 1
//#define BITMAPOBENY 30
#define BITMAPOBENX 50    // Faktor 2 2
#define BITMAPOBENY 21

//#define BITMAPUNTENX 78
//#define BITMAPUNTENY 120 
#define BITMAPUNTENX 62   // Faktor 2 2
#define BITMAPUNTENY 108 

#define BITMAPOBENW 40
#define BITMAPOBENH 30
#define BITMAPUNTENW 32
#define BITMAPUNTENH 30 
#define ANGLEX 61
#define ANGLEOBENY 81
#define ANGLEUNTENY 174
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

// ??????????
#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define FILE_WRITE  "w"
#define FILE_READ   "r"
#define FILE_APPEND "a"
