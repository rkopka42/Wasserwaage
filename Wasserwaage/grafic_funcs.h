/* grafic_funcs.h
 *  
 *  Polygonfill
 *  aus http://www-lehre.informatik.uni-osnabrueck.de/~cg/2000/skript/4_2_Scan_Line_Verfahren_f_252_r.html
 */

#define INT_MAX_VALUE 32767  // gibts da was fertiges ?

class Point {
  public:
  int x;
  int y;
};

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
        , uint16_t color=ST77XX_WHITE)
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
