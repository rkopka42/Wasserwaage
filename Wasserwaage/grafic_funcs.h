/* grafic_funcs.h
 *  
 *  Polygonfill
 *  aus http://www-lehre.informatik.uni-osnabrueck.de/~cg/2000/skript/4_2_Scan_Line_Verfahren_f_252_r.html
 */

#ifndef _GRAFIC_FUNCS_H
#define _GRAFIC_FUNCS_H

#include "Arduino.h"

#include "defines.h"
#include "grafics.h"

#ifdef USE_M5STACK
 #include <M5Stack.h>
#endif

// gleich wie xy - evt. zusammenführen
class Point {
  public:
  int x;
  int y;
};

void scan_line_fill(                    /* Fuellt das Innere eines Polygons */
        int NumPoints,                          /* Anzahl der Punkte                */
        Point Points[]                         /* Liste von Punkten                */
        , uint16_t color); 

void fillRect_(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void draw_value_v3_(int xpos,int ypos, float value, float max_value, int max_height, int breite, float line_value);
void draw_Bitmap_(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h,
            bool mirror, uint16_t color) ;
void draw_Bitmap_faktor(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h, int faktorw, int faktorh,
            bool mirror, uint16_t color);
void draw_vector(xy *form, int punkte, int x, int y, float angle, float factor, uint16_t color);
void draw_vector_fill(xy *form, int punkte, int x, int y, float angle, float factor, uint16_t color);
void show_display(void);

#endif
