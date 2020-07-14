/* grafic_funcs.h
 *  
 *  Polygonfill aus  opencv/modules/imgproc/src/drawing.cpp 
 * 
 */

// aus http://www-lehre.informatik.uni-osnabrueck.de/~cg/2000/skript/4_2_Scan_Line_Verfahren_f_252_r.html

#define INT_MAX_VALUE 32767

class Point {
  public:
  int x;
  int y;
};

/*
typedef struct {
  int x;
  int y;
} Point;
*/

// Womografiken

// alles relativ
Point seite_[] = {
  {0,0},   {-40,25},
  {15,0},   {5,10},   {10,0},   {5,-10},
  {30,0},   {5,10},   {10,0},   {5,-10},
  {15,0},   {0,-60},   {-90,0},   {0,20},  {20,0},   {-20,20},   {-10,0}, {0,20}
};

// alles relativ
Point back_[] = {
  {0,0},  {-30,30}, 
  {10,0},   {0,10},   {10,0},   {0,-10}, {20,0},  // l Rad
  {0,10},   {10,0},   {0,-10},  {10,0}, // r Rad
  
  {0,-60},   {-60,0},   {0,60}  // Aufbau noch ohne Spiegel
};

/** Klasse zur Implementation einer verzeigerten Kantenliste. */

/*public*/ class Edge {
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

        Edge(    int     y_top,
                        double  x_int,
                        int     delta_y,
                        double  delta_x,
                        Edge    *next) {

                this->y_top    = y_top;
                this->x_int    = x_int;
                this->delta_y  = delta_y;
                this->delta_x  = delta_x;
                this->next     = next;
        }

        /** Erzeugt ein Objekt vom Typ Kante mit den uebergebenen Parametern. */

        Edge(    int     y_top,
                        double  x_int,
                        int     delta_y,
                        double  delta_x) {

             //   (*this)(y_top, x_int, delta_y, delta_x, NULL);
             this->y_top    = y_top;
                this->x_int    = x_int;
                this->delta_y  = delta_y;
                this->delta_x  = delta_x;
                this->next     = NULL;
        }

        /** Erzeugt ein leeres Objekt vom Typ Kante. */

        Edge() {

              //  this(0, 0.0, 0, 0.0, NULL);
              this->y_top    = 0;
                this->x_int    = 0;
                this->delta_y  = 0.0;
                this->delta_x  = 0.0;
                this->next     = NULL;
        }
};
  

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

        if ((P2->y > P1.y) && (P2->y < y_next)) { P2->y--; x2=x2-dx; }   // Sonderfaelle
        if ((P2->y < P1.y) && (P2->y > y_next)) { P2->y++; x2=x2+dx; }

        dy=P2->y-P1.y;
        if (dy>0) { max_y = P2->y;
                    max_x = (double)x2;
                    dy++;
        } else    { max_y=P1.y;
                    max_x=(double)P1.x;
                    dy=1-dy;
        }

        Edge *edge1 = edges;                                           // Hilfsobjekt

        while (edge1->next->y_top >= max_y) edge1 = edge1->next;

        Edge *newedge = new Edge(max_y, max_x, dy, dx, edge1->next);    // einfuegen
                                                                      // sortiert nach
        edge1->next = newedge;                                         // max_y
}

 int Next_y(    /* liefert den y-Wert des naechsten Knoten laengs der Grenze */
                       /* dessen y-Koordinate verschieden ist von P[k].y            */
        int k,                                       /* Index des Punktes           */
        Point Points[],                              /* Liste von Punkten           */
        int n)                                       /* Anzahl der Punkte           */
{
        int compare_y, new_y;

        compare_y = Points[k].y;

        do {
                k = (k+1)%n;
                new_y = Points[k].y;
        } while (new_y == compare_y);

        return(new_y);
}

int Edge_Sort(                       /* erzeugt nach y sortierte Kantenliste */
                                             /* und liefert den kleinsten y-Wert     */
        int n,                               /* Anzahl der Punkte                    */
        Point P[],                           /* Punkteliste                          */
        Edge *edges)                          /* Kantenliste                          */
{
        int bottom_y;
        Point P1;

        Edge *edge1 = new Edge();

        edges->next=edge1;
        edge1->next=NULL;

        edges->y_top=INT_MAX_VALUE;
        edge1->y_top=-INT_MAX_VALUE;

        P1 = P[n-1];
        bottom_y = P1.y;

        for (int k=0; k<n; k++) {
                if (P1.y != P[k].y) Insert(edges,P1,P[k],Next_y(k,P,n));
                        else 
                        {
                          //set_dither_line(P1,P[k].x);   
                           tft_.drawLine(P1.x, P1.y,P[k].x, P1.y, ST77XX_WHITE);
                           DSerial.println(String(k) + "::: "+  String(P1.x)+ "/" + String(P1.y) + " --> " + String(P[k].x) + "/" + String(P1.y));

                        }
                if (P[k].y < bottom_y) bottom_y = P[k].y;
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
        while (l_act_edge->next->y_top >= scan) l_act_edge=l_act_edge->next;

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

        do {
                edge1=edges;
                while (edge1->next->x_int < edge2->next->x_int)
                        edge1=edge1->next;
                if (edge1 != edge2) {             // tausche edge1.next und edge2.next
                        edge3           = edge2->next->next;
                        edge2->next->next = edge1->next;
                        edge1->next      = edge2->next;
                        edge2->next      = edge3;
                        if (edge1->next==l_act_edge) l_act_edge=edge2;
                } else edge2 = edge2->next;

        } while (edge2 != l_act_edge);

        return (l_act_edge);
}

void Fill(                      /* generiert fuer je zwei Schnittpunkte   */
                                        /* aus der Kantenliste den Zeichne-Aufruf */
        Edge *edges,                     /* Beginn der aktuellen Kantenliste       */
        Edge *l_act_edge,                /* Ende der aktuellen Kantenliste         */
        int scan)                       /* Scanline                               */
{
        Point *Q = new Point();
        
        do 
        {
                edges = edges->next;
                Q->x = (int) (edges->x_int+0.5);
                Q->y = scan;
                edges = edges->next;
                //set_dither_line(Q,(int)(edges.x_int+0.5) );
//                tft_.drawLine(Q->x, Q->y, Q->x+(int)(edges->x_int+0.5), Q->y, ST77XX_WHITE);
  //              DSerial.println(String(scan) + ":: "+  String(Q->x)+ "/" + String(Q->y) + " --> " + String(Q->x+(int)(edges->x_int+0.5)) + "/" + String(Q->y));

                tft_.drawLine(Q->x, Q->y, (int)(edges->x_int+0.5), Q->y, ST77XX_WHITE);
                DSerial.println(String(scan) + ":: "+  String(Q->x)+ "/" + String(Q->y) + " --> " + String((int)(edges->x_int+0.5)) + "/" + String(Q->y));

        } 
        while (edges != l_act_edge);
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
        Point Points[])                         /* Liste von Punkten                */
{
        Edge *l_act_edge;
        int scan, bottom_y;

        Edge *edges = new Edge();

        bottom_y = Edge_Sort(NumPoints, Points, edges);

        l_act_edge = edges->next;

        for (scan = edges->next->y_top; scan >= bottom_y; scan--) 
        {
                l_act_edge = Update_List_Ptr(scan, l_act_edge);
                l_act_edge = Sort_Intersections(edges, l_act_edge);
                Fill (edges, l_act_edge, scan);
                l_act_edge = Update_Edges(edges, l_act_edge);
        }

        /* dispose dummies edges.next und edges */
        edges->next = NULL;
        edges = NULL;
}










#if 0
/****************************************************************************************\
*                                Polygons filling                                        *
\****************************************************************************************/

static inline void ICV_HLINE_X(uchar* ptr, int xl, int xr, const uchar* color, int pix_size)
{
    uchar* hline_min_ptr = (uchar*)(ptr) + (xl)*(pix_size);
    uchar* hline_end_ptr = (uchar*)(ptr) + (xr+1)*(pix_size);
    uchar* hline_ptr = hline_min_ptr;
    if (pix_size == 1)
      memset(hline_min_ptr, *color, hline_end_ptr-hline_min_ptr);
    else//if (pix_size != 1)
    {
      if (hline_min_ptr < hline_end_ptr)
      {
        memcpy(hline_ptr, color, pix_size);
        hline_ptr += pix_size;
      }//end if (hline_min_ptr < hline_end_ptr)
      size_t sizeToCopy = pix_size;
      while(hline_ptr < hline_end_ptr)
      {
        memcpy(hline_ptr, hline_min_ptr, sizeToCopy);
        hline_ptr += sizeToCopy;
        sizeToCopy = std::min(2*sizeToCopy, static_cast<size_t>(hline_end_ptr-hline_ptr));
      }//end while(hline_ptr < hline_end_ptr)
    }//end if (pix_size != 1)
}
//end ICV_HLINE_X()

static inline void ICV_HLINE(uchar* ptr, int xl, int xr, const void* color, int pix_size)
{
  ICV_HLINE_X(ptr, xl, xr, reinterpret_cast<const uchar*>(color), pix_size);
}
//end ICV_HLINE()

/* filling convex polygon. v - array of vertices, ntps - number of points */
static void
FillConvexPoly( Mat& img, const Point2l* v, int npts, const void* color, int line_type, int shift )
{
    struct
    {
        int idx, di;
        int64 x, dx;
        int ye;
    }
    edge[2];

    int delta = 1 << shift >> 1;
    int i, y, imin = 0;
    int edges = npts;
    int64 xmin, xmax, ymin, ymax;
    uchar* ptr = img.ptr();
    Size size = img.size();
    int pix_size = (int)img.elemSize();
    Point2l p0;
    int delta1, delta2;

    if( line_type < CV_AA )
        delta1 = delta2 = XY_ONE >> 1;
    else
        delta1 = XY_ONE - 1, delta2 = 0;

    p0 = v[npts - 1];
    p0.x <<= XY_SHIFT - shift;
    p0.y <<= XY_SHIFT - shift;

    assert( 0 <= shift && shift <= XY_SHIFT );
    xmin = xmax = v[0].x;
    ymin = ymax = v[0].y;

    for( i = 0; i < npts; i++ )
    {
        Point2l p = v[i];
        if( p.y < ymin )
        {
            ymin = p.y;
            imin = i;
        }

        ymax = std::max( ymax, p.y );
        xmax = std::max( xmax, p.x );
        xmin = MIN( xmin, p.x );

        p.x <<= XY_SHIFT - shift;
        p.y <<= XY_SHIFT - shift;

        if( line_type <= 8 )
        {
            if( shift == 0 )
            {
                Point pt0, pt1;
                pt0.x = (int)(p0.x >> XY_SHIFT);
                pt0.y = (int)(p0.y >> XY_SHIFT);
                pt1.x = (int)(p.x >> XY_SHIFT);
                pt1.y = (int)(p.y >> XY_SHIFT);
                Line( img, pt0, pt1, color, line_type );
            }
            else
                Line2( img, p0, p, color );
        }
        else
            LineAA( img, p0, p, color );
        p0 = p;
    }

    xmin = (xmin + delta) >> shift;
    xmax = (xmax + delta) >> shift;
    ymin = (ymin + delta) >> shift;
    ymax = (ymax + delta) >> shift;

    if( npts < 3 || (int)xmax < 0 || (int)ymax < 0 || (int)xmin >= size.width || (int)ymin >= size.height )
        return;

    ymax = MIN( ymax, size.height - 1 );
    edge[0].idx = edge[1].idx = imin;

    edge[0].ye = edge[1].ye = y = (int)ymin;
    edge[0].di = 1;
    edge[1].di = npts - 1;

    edge[0].x = edge[1].x = -XY_ONE;
    edge[0].dx = edge[1].dx = 0;

    ptr += img.step*y;

    do
    {
        if( line_type < CV_AA || y < (int)ymax || y == (int)ymin )
        {
            for( i = 0; i < 2; i++ )
            {
                if( y >= edge[i].ye )
                {
                    int idx0 = edge[i].idx, di = edge[i].di;
                    int idx = idx0 + di;
                    if (idx >= npts) idx -= npts;
                    int ty = 0;

                    for (; edges-- > 0; )
                    {
                        ty = (int)((v[idx].y + delta) >> shift);
                        if (ty > y)
                        {
                            int64 xs = v[idx0].x;
                            int64 xe = v[idx].x;
                            if (shift != XY_SHIFT)
                            {
                                xs <<= XY_SHIFT - shift;
                                xe <<= XY_SHIFT - shift;
                            }

                            edge[i].ye = ty;
                            edge[i].dx = ((xe - xs)*2 + (ty - y)) / (2 * (ty - y));
                            edge[i].x = xs;
                            edge[i].idx = idx;
                            break;
                        }
                        idx0 = idx;
                        idx += di;
                        if (idx >= npts) idx -= npts;
                    }
                }
            }
        }

        if (edges < 0)
            break;

        if (y >= 0)
        {
            int left = 0, right = 1;
            if (edge[0].x > edge[1].x)
            {
                left = 1, right = 0;
            }

            int xx1 = (int)((edge[left].x + delta1) >> XY_SHIFT);
            int xx2 = (int)((edge[right].x + delta2) >> XY_SHIFT);

            if( xx2 >= 0 && xx1 < size.width )
            {
                if( xx1 < 0 )
                    xx1 = 0;
                if( xx2 >= size.width )
                    xx2 = size.width - 1;
                ICV_HLINE( ptr, xx1, xx2, color, pix_size );
            }
        }
        else
        {
            // TODO optimize scan for negative y
        }

        edge[0].x += edge[0].dx;
        edge[1].x += edge[1].dx;
        ptr += img.step;
    }
    while( ++y <= (int)ymax );
}
#endif
