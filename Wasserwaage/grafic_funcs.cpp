/* grafic_funcs.cpp
 *  
 *  Polygonfill
 *  aus http://www-lehre.informatik.uni-osnabrueck.de/~cg/2000/skript/4_2_Scan_Line_Verfahren_f_252_r.html
 */

#include "grafic_funcs.h"

#ifdef USE_DISPLAY

// Funktionen für TFT Anzeige

#define INT_MAX_VALUE 32767  // gibts da was fertiges ?

#define ST77XX_WHITE_ 0xFFFF

/** Klasse zur Implementation einer verzeigerten Kantenliste. */
 class Edge {
  public:
  /** groesste y-Koordinate der Kante. */
  int      y_top;

  /** Schnittpunkt der Scan-Line mit der Kante. */
  double   x_int;

  /** y-Ausdehnung der Kante. */
  int      delta_y;

  /** inverse Steigung 1/s der Kante. */
  double   delta_x;

  /** naechste Kante in der Kantenliste. */
  Edge     *next;

  /** Erzeugt ein Objekt vom Typ Kante mit den uebergebenen Parametern.
    * next ist die naechste Kante in der Liste.                         */

  Edge(   int     y_top,
          double  x_int,
          int     delta_y,
          double  delta_x,
          Edge    *next) 
  {
    this->y_top    = y_top;
    this->x_int    = x_int;
    this->delta_y  = delta_y;
    this->delta_x  = delta_x;
    this->next     = next;
  }

  /** Erzeugt ein Objekt vom Typ Kante mit den uebergebenen Parametern. */

  Edge( int     y_top,
        double  x_int,
        int     delta_y,
        double  delta_x) 
  {
 //   (*this)(y_top, x_int, delta_y, delta_x, NULL);
    this->y_top    = y_top;
    this->x_int    = x_int;
    this->delta_y  = delta_y;
    this->delta_x  = delta_x;
    this->next     = NULL;
  }

  /** Erzeugt ein leeres Objekt vom Typ Kante. */

  Edge() 
  {
    //  this(0, 0.0, 0, 0.0, NULL);
    this->y_top    = 0;
    this->x_int    = 0;
    this->delta_y  = 0.0;
    this->delta_x  = 0.0;
    this->next     = NULL;
  }
};
  
static Edge *zeiger[30];  // Kantenzahl, nicht größer als Punktezahl
static int zeigercnt=0;

/*************************************************************************************/
/*                                                                                   */
/*                  fuellt mit dem Scan-Line-Algorithmus das Innere eines Polygons   */
/*                                                                                   */
/*************************************************************************************/
       void Insert(                     /* fuegt in die Liste eine Kante ein */
        Edge *edges,                             /* Beginn der Kantenliste            */
        Point P1, Point p2,                     /* einzufuegende Kante (P1,P2)       */
        int y_next)                             /* Behandlung von Sonderfaellen:     */
                                                /* siehe Prozedur Next_y             */
{
  int max_y, dy;
  double x2, dx, max_x;

  Point *P2 = new Point(p2);
  
  dx=(double)(P2->x-P1.x)/(double)(P2->y-P1.y);
  x2=(double) P2->x;

  if ((P2->y > P1.y) && (P2->y < y_next))  { P2->y--; x2=x2-dx; }   // Sonderfaelle
  if ((P2->y < P1.y) && (P2->y > y_next))  { P2->y++; x2=x2+dx; }

  dy=P2->y-P1.y;
  if (dy>0) 
  { 
    max_y = P2->y;
    max_x = (double)x2;
    dy++;
  } 
  else    
  { 
    max_y=P1.y;
    max_x=(double)P1.x;
    dy=1-dy;
  }

  Edge *edge1 = edges;                                           // Hilfsobjekt

  while (edge1->next->y_top >= max_y) edge1 = edge1->next;

  Edge *newedge = new Edge(max_y, max_x, dy, dx, edge1->next);    // einfuegen

  zeiger[zeigercnt]=newedge;
  zeigercnt++;

  // sortiert nach
  edge1->next = newedge;                                         // max_y
  delete(P2);
}

 int Next_y(    /* liefert den y-Wert des naechsten Knoten laengs der Grenze */
                       /* dessen y-Koordinate verschieden ist von P[k].y            */
        int k,                                       /* Index des Punktes           */
        Point Points[],                              /* Liste von Punkten           */
        int n)                                       /* Anzahl der Punkte           */
{
  int compare_y, new_y;

  compare_y = Points[k].y;

  do 
  {
    k = (k+1)%n;
    new_y = Points[k].y;
  } 
  while (new_y == compare_y);

  return(new_y);
}

int Edge_Sort(                       /* erzeugt nach y sortierte Kantenliste */
                                             /* und liefert den kleinsten y-Wert     */
        int n,                               /* Anzahl der Punkte                    */
        Point P[],                           /* Punkteliste                          */
        Edge *edges                          /* Kantenliste                          */
        , uint16_t color=ST77XX_WHITE)
{
  int bottom_y;
  Point P1;

  Edge *edge1 = new Edge();

  zeiger[zeigercnt]=edge1;
  zeigercnt++;

  edges->next=edge1;
  edge1->next=NULL;

  edges->y_top=INT_MAX_VALUE;
  edge1->y_top=-INT_MAX_VALUE;

  P1 = P[n-1];
  bottom_y = P1.y;

  for (int k=0; k<n; k++) 
  {
    if (P1.y != P[k].y) 
      Insert(edges,P1,P[k],Next_y(k,P,n));
    else 
    {
       tft_.drawFastHLine(P1.x, P1.y,P[k].x-P1.x , color);
    }
    if (P[k].y < bottom_y)  bottom_y = P[k].y;
    P1 = P[k];
  }
  return (bottom_y);        
}

Edge *Update_List_Ptr(              /* aktualisiert den Zeiger auf die letzte */
                                           /* aktive Kante und gibt ihn zurueck      */
                                           /* wegen Dekrementieren der Scan-Line     */
                                           /* werden einige Kanten aktiv             */
        int scan,
        Edge *l_act_edge)
{
  while (l_act_edge->next->y_top >= scan) 
    l_act_edge=l_act_edge->next;

  return(l_act_edge);
}


Edge *Sort_Intersections (    /* sortiert die aktive Kantenliste       */
        Edge *edges,                         /* Beginn der Kantenliste                */
        Edge *l_act_edge)                    /* Ende der Kantenliste                  */
                                            /* Liefert den ggf. modifizierten Zeiger */
                                            /* auf die letzte aktive Kante zurueck   */
{
  Edge *edge1, *edge2, *edge3;

  edge2 = edges->next;

  do 
  {
    edge1=edges;
    while (edge1->next->x_int < edge2->next->x_int)
      edge1=edge1->next;
    if (edge1 != edge2) 
    {             // tausche edge1.next und edge2.next
      edge3           = edge2->next->next;
      edge2->next->next = edge1->next;
      edge1->next      = edge2->next;
      edge2->next      = edge3;
      if (edge1->next==l_act_edge) l_act_edge=edge2;
    } 
    else  edge2 = edge2->next;
  } 
  while (edge2 != l_act_edge);

  return (l_act_edge);
}

void Fill(                      /* generiert fuer je zwei Schnittpunkte   */
                                        /* aus der Kantenliste den Zeichne-Aufruf */
        Edge *edges,                     /* Beginn der aktuellen Kantenliste       */
        Edge *l_act_edge,                /* Ende der aktuellen Kantenliste         */
        int scan                       /* Scanline                               */
        , uint16_t color=ST77XX_WHITE)
{
  Point *Q = new Point();   // wieso new nötig ?
  
  do 
  {
    edges = edges->next;
    Q->x = (int) (edges->x_int+0.5);
    Q->y = scan;
    edges = edges->next;

    tft_.drawFastHLine(Q->x, Q->y, (int)(edges->x_int+0.5)-Q->x, color); // bringt nicht so viel
  } 
  while (edges != l_act_edge);
  delete(Q);
}

Edge *Update_Edges(/* aktual. die aktiven Kanten in der Kantenliste   */
        Edge *edges,              /* beginnend bei edges                             */
        Edge *l_act_edge)         /* und endend bei l_act_edge                       */
                                 /* Kanten mit delta_y=0 werden entfernt. Der ggf.  */
                                 /* modifizierte Zeiger auf die letzte aktive Kante */
                                 /* wird zurueckgegeben                             */
{
  Edge *prev_edge;

  prev_edge = edges;

  do 
  {
    edges = prev_edge->next;
    if (edges->delta_y > 1) 
    {
      edges->delta_y--;
      edges->x_int = edges->x_int - edges->delta_x;
      prev_edge = edges;
    } 
    else
    {
      prev_edge->next = edges->next;
      if (edges == l_act_edge) l_act_edge = prev_edge;
      edges = NULL;        /* dispose edges */
    } /* if */
  } 
  while (prev_edge != l_act_edge);
  return (l_act_edge);
}

void scan_line_fill(                    /* Fuellt das Innere eines Polygons */
        int NumPoints,                          /* Anzahl der Punkte                */
        Point Points[]                         /* Liste von Punkten                */
        , uint16_t color=ST77XX_WHITE_)
{
  Edge *l_act_edge;
  int scan, bottom_y;

  memset(zeiger, sizeof(zeiger),0);
  zeigercnt=0;
  
  Edge *edges = new Edge();   // ist das der Start ?
  
  zeiger[zeigercnt]=edges;
  zeigercnt++;

  bottom_y = Edge_Sort(NumPoints, Points, edges, color);

  Edge *edges_ = edges;

  l_act_edge = edges->next;

  for (scan = edges->next->y_top; scan >= bottom_y; scan--) 
  {
    l_act_edge = Update_List_Ptr(scan, l_act_edge);
    l_act_edge = Sort_Intersections(edges, l_act_edge);
    Fill (edges, l_act_edge, scan, color);
    l_act_edge = Update_Edges(edges, l_act_edge);
  }

  // Quelle war JAVA, wo es kein delete() gibt, daher etwas umständlich
  for (int n=0; n<zeigercnt; n++)
  {
    delete(zeiger[n]); 
  }
}


void fillRect_(int16_t x, int16_t y, int16_t w, int16_t h,
          uint16_t color) 
{
  // evt. in früheren Versionen von LCD/TFT nicht verfügbar
  tft_.fillRect(x, y, w, h, color);
/*
  for (int16_t i=x; i<x+w; i++) 
  {
    //tft_.drawLine(i, y, i, y+h, color);
    tft_.drawFastVLine(i,y,h,color);
  }
  */
}


//vertikales Rechteck nach oben
// xpos ypos gibt den unteren linken Punkt an
// bei line_value gibt es einen inversen Strich
void draw_value_v3_(int xpos,int ypos, float value, float max_value, int max_height, int breite, float line_value)
{
    int16_t hoehe, strich;
    int color=0;
    bool too_much=false;
    
    if (value > max_value)  
    {
      value=max_value;  // evt. Kennung für Überlauf
      too_much=true;
    }
    
    hoehe  = value     /max_value * max_height;
    strich = line_value/max_value * max_height;
    
    if (too_much)
    {
      fillRect_    (xpos , ypos-hoehe, breite           , hoehe      , ST77XX_RED); 
      tft_.drawLine(xpos, ypos-strich, xpos+breite-1    , ypos-strich, ST77XX_BLACK);
    }      
    else
    {
      fillRect_      (xpos , ypos-max_height, breite , max_height-hoehe, ST77XX_BLACK);       
      if (line_value>value)
      {
        fillRect_    (xpos, ypos-hoehe,  breite       , hoehe      , ST77XX_GREEN); 
        tft_.drawLine(xpos, ypos-strich, xpos+breite-1, ypos-strich, ST77XX_GREEN); 
      }
      else
      {
        fillRect_    (xpos, ypos-hoehe , breite       , hoehe      , ST77XX_YELLOW); 
        tft_.drawLine(xpos, ypos-strich, xpos+breite-1, ypos-strich, ST77XX_BLACK);
      }
    }
}
         
void draw_Bitmap_(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h,
            bool mirror=false, uint16_t color=ST77XX_WHITE) 
{
  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) 
      {
        if (mirror)
          tft_.drawPixel(x+w-i-1, y+j, ST77XX_BLACK);
        else
          tft_.drawPixel(x+i, y+j, ST77XX_BLACK);
      }
      else
      {
        if (mirror)
          tft_.drawPixel(x+w-i-1, y+j, color);
        else
          tft_.drawPixel(x+i, y+j, color);
      }
    }
  }
}

/* h und w geben die Maße des Bildes an
 * x,y den Startpunkt 
 * faktorw und faktorh geben die Vergrößerung an (wie wäre das mit float ?)
 */
void draw_Bitmap_faktor(int16_t x, int16_t y,
            const uint8_t *bitmap, int16_t w, int16_t h, int faktorw, int faktorh,
            bool mirror=false, uint16_t color=ST77XX_WHITE) 
{
  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<(h*faktorh); j++)  // Zeilen
  {
    for(i=0; i<(w*faktorw); i++ )   // Spalten
    {
      if (pgm_read_byte(bitmap + (j/faktorh) * byteWidth + (i/faktorw) / 8) & (128 >> ((i/faktorw) & 7))) 
      {
        if (mirror)
          tft_.drawPixel(x+w-i-1, y+j, ST77XX_BLACK); // Hintergrund löschen (schwarz)
        else
          tft_.drawPixel(x+i, y+j, ST77XX_BLACK);
      }
      else
      {
        if (mirror)
          tft_.drawPixel(x+w-i-1, y+j, color);
        else
          tft_.drawPixel(x+i, y+j, color);
      }
    }
  }
}

// Erste Linie unsichtbar !!!
void draw_vector(xy *form, int punkte, int x, int y, float angle=0, float factor=1, uint16_t color=ST77XX_WHITE)
{ 
 int n;//=0;
 float lastx, lasty;
 float nextx, nexty;

// fillRect_      (x-50 , y-43, 100 ,88, ST77XX_BLACK);  //oder den alten mit sw ? dann muß man den aber merken
 // erstmal maximale Werte errechnen: einfach wie unten, aber nur max bestimmen
 
 lastx = x + form[0].x; // Startpunkt + erster Vektor
 lasty = y + form[0].y;

 for (n=1; n<punkte; n++)
 {
    //DSerial.print("N="+String(n)+ " ");
    nextx = lastx + form[n].x*factor * cos(angle*PI/180) - form[n].y*factor * sin(angle*PI/180);
    nexty = lasty + form[n].x*factor * sin(angle*PI/180) + form[n].y*factor * cos(angle*PI/180);

    if (n>1)
    {
      tft_.drawLine(lastx, lasty, nextx, nexty, color); 
   //   DSerial.println(String(n) + ": x=" + String(lastx) + " y=" +String(lasty)+ " -> x=" + String(nextx) + " y=" +String(nexty));      
    }
    lastx = nextx;
    lasty = nexty;
 }
}

void draw_vector_fill(xy *form, int punkte, int x, int y, float angle=0, float factor=1, uint16_t color=ST77XX_WHITE)
{ 
  int n;
  float lastx, lasty;
  float nextx, nexty;

  Point seite_[punkte+1];

  seite_[0].x = lastx = x + form[0].x; // Startpunkt + erster Vektor
  seite_[0].y = lasty = y + form[0].y;

  for (n=1; n<punkte; n++)
  {
    //DSerial.print("N="+String(n)+ " ");
    seite_[n].x =    nextx = lastx + form[n].x*factor * cos(angle*PI/180) - form[n].y*factor * sin(angle*PI/180);
    seite_[n].y =    nexty = lasty + form[n].x*factor * sin(angle*PI/180) + form[n].y*factor * cos(angle*PI/180);
    lastx = nextx;
    lasty = nexty;
  }
  seite_[n]=seite_[0];  // Endpunkt zum Abschliessen dazufügen, Canvas macht das automatisch. Evt. hier berücksichtigen und nur machen, wenn nötig
  
  scan_line_fill( punkte+1, seite_, color);
}

void show_display(void)
{
  static bool first=true; 
  char buf[10];
  tft_.setTextSize(2);            
  static float last_fp_corr=0, last_fr_corr=0;

  tft_.setTextColor(ST77XX_WHITE,ST77XX_BLACK);  
  get_height(buf, int(z4), MAX_SHOW,true);      // Radhöhe links vorne
  tft_.setCursor(0,0);
  tft_.print(buf);

  if (nb>0)                                     // WLAN Anzeige links oben, wenn Clients da sind
  {
    sprintf(buf, "W:%1d", nb);
    tft_.setCursor(TEXTHALBLINKS,0);
    tft_.setTextColor(ST77XX_BLUE,ST77XX_RED);  
    tft_.print(buf);
  }
  else
  {
    tft_.setCursor(TEXTHALBLINKS,0);
    tft_.setTextColor(ST77XX_BLACK,ST77XX_BLACK);  
    tft_.print("   ");    
  }
  
  if (do_calib)
    strcpy(buf,"cal");
  else if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK ) // test for OK
    strcpy(buf,"OK ");
  else
    strcpy(buf,"   ");
  tft_.setTextColor(ST77XX_BLUE,ST77XX_BLACK);  // Meldung oben Mitte
  tft_.setCursor(TEXTMITTEOBENX,0);
  tft_.print(buf);  // Hier nur, wenn zuletzt etwas drin stand, sonst überschreibt es die Grafik
  
  get_height(buf, int(z1), MAX_SHOW,false);     // Radhöhe rechts vorne
  tft_.setTextColor(ST77XX_WHITE,ST77XX_BLACK);           
  tft_.setCursor(TEXTRECHTSX,0);
  tft_.print(buf);
  
  get_height(buf, int(z3), MAX_SHOW,true);       // Radhöhe links unten
  tft_.setCursor(0,TEXTUNTENY);
  tft_.print(buf);
  
  get_height(buf, int(z2), MAX_SHOW,false);      // Radhöhe rechts hinten
  tft_.setCursor(TEXTRECHTSX,TEXTUNTENY);
  tft_.print(buf);

  // Balken außen
  
  // void draw_value_v3(int xpos,int ypos, float value, float max_value, int max_height, int breite, strich)
  draw_value_v3_(00 ,VALUEOBENY , z4, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  draw_value_v3_(VALUERECHTSX,VALUEOBENY , z1, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  draw_value_v3_(00 ,VALUEUNTENY, z3, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  draw_value_v3_(VALUERECHTSX,VALUEUNTENY, z2, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);

  // Bestimmung der Bitmap für das Womo, bei Vektor oder Fill nur die Farbe genutzt
  
  // Zeiger über Feld ? Dann ein Aufruf
  const uint8_t *p_map;
  uint16_t color=ST77XX_GREEN;
  
  if (fp_corr >=3)
  {
    p_map = Auto2_0_10f;
    color=ST77XX_RED;
  }
  else if (fp_corr >=1)
  {
    p_map = Auto2_0_05f;
    color=ST77XX_YELLOW;
  }
  else if (fp_corr <=-3)
  {
    p_map = Auto2_10f;
    color=ST77XX_RED;
  }
  else if (fp_corr <=-1)
  {
    p_map = Auto2_05f;
    color=ST77XX_YELLOW;
  }
  else
  {
    p_map = Auto2_00f;
    color=ST77XX_GREEN;
  }

  bool mirror_=(int(fp_corr) >0);  // int gegen Flackern rund um den Nullwert
  
// Winkel multiplizieren, aber auch begrenzen - gibts in der Realität nicht, aber trotzdem min max ?
  static float last_fp_corr_;   // Wert aufheben für das Löschen beim nächsten Mal
  float fp_corr_;               // Wert für die Anzeige vergrößern und begrenzen

  if (mirror_)
  {
    fp_corr_ = min((float)int(fp_corr*5),(float)45.0);
  }
  else
  {
    fp_corr_ = max((float)int(fp_corr*5),(float)-45.0);
  }
      
  //draw_Bitmap_faktor(BITMAPOBENX, BITMAPOBENY, p_map, BITMAPOBENW, BITMAPOBENH, 2,2, false, color);       
  
  if ( abs(last_fp_corr_-fp_corr_)>0.1 or first)  // float, also abs() und Schwelle dazu
  {   
  //  draw_vector(seite, sizeof(seite)/sizeof(xy), VECTOROBENX ,VECTOROBENY, min((float)int(-last_fp_corr*5), (float)45), 0.7, ST77XX_BLACK);
  //  draw_vector(seite, sizeof(seite)/sizeof(xy), VECTOROBENX ,VECTOROBENY, min((float)int(-fp_corr*5),(float)45), 0.7, color);
    draw_vector_fill(seite, sizeof(seite)/sizeof(xy), VECTOROBENX ,VECTOROBENY, -last_fp_corr_, FAKTOR_OBEN, ST77XX_BLACK);
    draw_vector_fill(seite, sizeof(seite)/sizeof(xy), VECTOROBENX ,VECTOROBENY, -fp_corr_, FAKTOR_OBEN, color);
 
    last_fp_corr_=fp_corr_;
  }

  // Bitmap nicht mehr nötig, aber Farben
  if (fr_corr >=3 or fr_corr <=-3)
  {
    p_map = Auto3_10f;
    color=ST77XX_RED;
  }
  else if (fr_corr >=1 or fr_corr <=-1)
  {
    p_map = Auto3_05f;
    color=ST77XX_YELLOW;
  }
  else
  {
    p_map = Auto3_00f;
    color=ST77XX_GREEN;
  }
    
  mirror_=(int(fr_corr) >0);  // int gegen Flackern
/*
  if (!mirror_)
    draw_Bitmap_faktor(BITMAPUNTENX, BITMAPUNTENY, p_map, BITMAPUNTENW, BITMAPUNTENH, 2,2, mirror_, color);          
  else
    draw_Bitmap_faktor(BITMAPUNTENX+22, BITMAPUNTENY, p_map, BITMAPUNTENW, BITMAPUNTENH, 2,2, mirror_, color);          
*/
  //draw_vector(seite, sizeof(seite)/sizeof(xy), VECTOROBENX ,VECTOROBENY, (float)int(-last_fp_corr*5), 1, ST77XX_BLACK);  

  static float last_fr_corr_;
  float fr_corr_;

  if (mirror_)
  {
    fr_corr_ = min((float)int(fr_corr*5),(float)45.0);
  }
  else
  {
    fr_corr_ = max((float)int(fr_corr*5),(float)-45.0);
  }
  
  if ( abs(last_fr_corr_-fr_corr_)>0.1 or first)  // float, also abs() und Schwelle dazu
  {   
    //  draw_vector(back, sizeof(back)/sizeof(xy), VECTORUNTENX ,VECTORUNTENY, min((float)int(last_fr_corr*5),(float)45.0), 1, ST77XX_BLACK);
    //  draw_vector(back, sizeof(back)/sizeof(xy), VECTORUNTENX ,VECTORUNTENY, min((float)int(fr_corr*5),(float)45.0), 1, color);
    draw_vector_fill(back, sizeof(back)/sizeof(xy), VECTORUNTENX ,VECTORUNTENY, last_fr_corr_, FAKTOR_UNTEN, ST77XX_BLACK);
    draw_vector_fill(back, sizeof(back)/sizeof(xy), VECTORUNTENX ,VECTORUNTENY, fr_corr_, FAKTOR_UNTEN, color);
    last_fr_corr_=fr_corr_;
  }
  
  tft_.setCursor(ANGLEX,ANGLEOBENY);
 // tft_.setTextSize(3);
  get_angle(buf, fp_corr);
  tft_.print(buf+ String("  "));
 // tft_.setTextSize(2);
  
  tft_.setCursor(ANGLEX,ANGLEUNTENY);
  get_angle(buf, fr_corr);
  tft_.print(buf+ String("  "));

  first=false;
}
#endif
