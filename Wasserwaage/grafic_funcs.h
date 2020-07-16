/* grafic_funcs.h
 *  
 *  Polygonfill
 *  aus http://www-lehre.informatik.uni-osnabrueck.de/~cg/2000/skript/4_2_Scan_Line_Verfahren_f_252_r.html
 */

#ifndef _GRAFIC_FUNCS_H
#define _GRAFIC_FUNCS_H

#include "Arduino.h"

#include "defines.h"

#ifdef USE_M5STACK
 #include <M5Stack.h>
#endif

class Point {
  public:
  int x;
  int y;
};

void scan_line_fill(                    /* Fuellt das Innere eines Polygons */
        int NumPoints,                          /* Anzahl der Punkte                */
        Point Points[]                         /* Liste von Punkten                */
        , uint16_t color); 

#endif
