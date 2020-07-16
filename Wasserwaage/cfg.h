/* cfg.h  Configuration and ini file
 * 
 */

#ifndef _CFG_H
#define _CFG_H

#include "Arduino.h"
#include "SPIFFS.h"

#include "defines.h"

#define TYPE_INT    1
#define TYPE_STRING 2
#define TYPE_FLOAT  3
#define TYPE_INT_HIDDEN    4
#define TYPE_STRING_HIDDEN 5
#define TYPE_FLOAT_HIDDEN  6
#define TYPE_DONT_USE      7  // Werte die im Feld stehen und abgespeichert werden, aber nicht verändert werden durch die Funktionen -> bei FORM weglassen, sonst schon anzeigen ???
#define TYPE_INT_OPTIONS   8   // dann sind die Optionen im addon -> noch unklar
#define TYPE_INT_DONT_USE  9  // nicht bei FORM, aber bei read defaults schon, daher Typ nötig

#define OUTPUT_SERIAL 0
#define OUTPUT_TEXT   1
#define OUTPUT_HTML   2
#define OUTPUT_FILE   3
#define OUTPUT_FORM   4

/*
typedef struct {
  int n;
  //String text;
  int value;
} option_t;
*/

typedef struct {
    void *value;    // Zeiger auf die Variable
    float min;      // float used as int for the other types
    float max;      // max string length / highest value
    String def;     // kann auch Zahlen enthalten
    int typ;        // type of value TYPE_...
    String name;    // für File
    String text;    // Hilfetext für Eingabe
    void *addon;    // Zeiger aufs Hilfsinformationen z.B. für Auswahlfelder
}  conf_t;

int read_defaults(conf_t cfg[], int cfg_elements);
int read_ini(String filename, conf_t cfg[], int cfg_elements);
int write_ini(String filename, conf_t cfg[], int cfg_elements, int outputtype, String &output);
bool getvaluesFromParams_(conf_t cfg[], int cfg_elements) ;

#endif
