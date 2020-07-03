/*  utils.h  Hilfsfunktionen für Wasserwaage_WLAN_xx
 * 
 */

static float  keilhoehe=KEIL_HOEHE;

String farben[]= {
  "ctx.fillStyle = \"rgb(0,200,0)\";\n",
  "ctx.fillStyle = \"rgb(200,0,0)\";\n",
  "ctx.fillStyle = \"rgb(200,200,0)\";\n",
  "ctx.fillStyle = \"rgb(0,0,200)\";\n"
};

// Zahlenausgabe für Höhenwerte
void get_height(char *buf_, int val, int max_val, bool left)
{
  if (val<=max_val)        
  {
    if (left)
      sprintf(buf_,"%02d ",val);
    else
      sprintf(buf_," %02d",val);
  }
  else
    sprintf(buf_,">%02d",max_val);
}

// Darstellung der Winkel - geht sicher simpler
/*  -0.2
 *  -10
 *   10
 *   0.2
 */  
void get_angle(char *buf, float value)
{
  String s;
  
  if (value < 0)  s=String("-");
  else            s=String(" ");
  
  s += String(abs(value), (abs(value) >=10? 0:1) );
  if (abs(value) >=10)  s += " ";
  strcpy(buf, s.c_str() );
}       

String get_angle(float value)
{
  static String s;
  
  if (value < 0)  s=String("-");
  else            s=String(" ");
  
  s += String(abs(value), (abs(value) >=10? 0:1) );
  if (abs(value) >=10)  s += " ";
  DSerial.println(s);
  return s;
}       

// Canvas 

//vertikales Rechteck nach oben
// xpos ypos gibt den unteren linken Punkt an
// bei line_value gibt es einen inversen Strich
//ctx.fillRect (30, 18, 30, 200);  // x y b h
void draw_value_v3(int xpos,int ypos, float value, float max_value, int max_height, int breite, float line_value, String &s)
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

    s = "ctx.fillStyle = \"rgb(0,0,0)\";\n";
    s += "ctx.beginPath();\n";
    s += "ctx.moveTo(" + String(xpos-1+breite)              + " , " + String(ypos-strich+max_height)               + " );\n";
    s += "ctx.lineTo(" + String(xpos-1+DREIECK_KANTE+breite)+ " , " + String(ypos-strich-DREIECK_KANTE+max_height) + " );\n";
    s += "ctx.lineTo(" + String(xpos-1+DREIECK_KANTE+breite)+ " , " + String(ypos-strich+DREIECK_KANTE+max_height) + " );\n";
    s += "ctx.fill();\n";

    s+="ctx.fillRect (";
    s+= String(xpos) + ", ";
    s+= String(ypos+ max_height) + ", ";
    s+= String(breite) + ", ";
    s+= String(1) + "); \n";
    
    if (too_much)
    {
      s+="ctx.fillRect (";
      s+= String(xpos) + ", ";
      s+= String(ypos) + ", ";
      s+= String(breite) + ", ";
      s+= String(-10) + "); \n";
    
      s+="ctx.fillStyle = \"rgb(200,0,0)\";\n";
    }      
    else
    {    
      if (line_value>value)  s+="ctx.fillStyle = \"rgb(0,200,0)\";\n";
      else                   s+="ctx.fillStyle = \"rgb(200,200,0)\";\n";
    }

    s+="ctx.fillRect (";
    s+= String(xpos) + ", ";
    s+= String(ypos+max_height-hoehe) + ", ";
    s+= String(breite) + ", ";
    s+= String(hoehe) + "); \n";
    //DSerial.println(s);
}

// Ein Punkt einer gefüllten (oder hohlen) Struktur
typedef struct {
 int x;
 int y;  
} xy;

// Womografiken

// alles relativ
xy seite[] = {
  {0,0},   {-40,25},
  {15,0},   {5,10},   {10,0},   {5,-10},
  {30,0},   {5,10},   {10,0},   {5,-10},
  {15,0},   {0,-60},   {-90,0},   {0,20},  {20,0},   {-20,20},   {-10,0}, {0,20}
};

// alles relativ
xy back[] = {
  {0,0},  {-30,30}, 
  {10,0},   {0,10},   {10,0},   {0,-10}, {20,0},  // l Rad
  {0,10},   {10,0},   {0,-10},  {10,0}, // r Rad
  
  {0,-60},   {-60,0},   {0,60}  // Aufbau noch ohne Spiegel
};

// alles relativ
xy pfeil[] = {
  {0,0},  {0,20}, 
  {-30,-20}, {30,-120}, {30,+120}, {-30,20}
};

// Erste Linie unsichtbar !!!
void draw_pic(String &s, xy *form, int punkte, int x, int y, float angle=0, float factor=1)
{ 
 int n;//=0;
 float lastx, lasty;
 float nextx, nexty;
 
 s="ctx.beginPath();\n";

 lastx = x + form[0].x; // Startpunkt + erster Vektor
 lasty = y + form[0].y;
 //s+= "ctx.moveTo(" + String(int(lastx)) +", "+ String(int(lasty)) +");\n";

 for (n=1; n<punkte; n++)
 {
    //DSerial.print("N="+String(n)+ " ");
    nextx = lastx + form[n].x*factor * cos(angle*PI/180) - form[n].y*factor * sin(angle*PI/180);
    nexty = lasty + form[n].x*factor * sin(angle*PI/180) + form[n].y*factor * cos(angle*PI/180);

    if (n<=1)
      s+= "ctx.moveTo(" + String(int(nextx)) +", "+ String(int(nexty)) +");\n";
    else
      s+= "ctx.lineTo(" + String(int(nextx)) +", "+ String(int(nexty)) +");\n";
    lastx = nextx;
    lasty = nexty;
 }
 s+="ctx.fill();\n";
}

/*
void dumpClients()
{
  int nb = WiFi.softAPgetStationNum();
  DSerial.print("Stations connected = ");
  DSerial.println(WiFi.softAPgetStationNum());
}
*/

// Adresse des HDMMI01
#define  COMP_I2ADDR  0x30 
#define  TakeMeasure  0x01 

int maxX=2124, minX=1947, maxY=2133, minY=1944;
float mygrad=0;

int comp_x,comp_y; 

// Werte in globalen Variablen 
// return 0-OK -1 -2 -3 Fehler bei I2C
int get_compass()
{ 
  byte MsbX,LsbX,MsbY,LsbY; 
  int  x,y; 
  char n; 

#ifdef USE_M5KOMPASS
  DSerial.print(" Comp: ");
  DSerial.print(comp_x);
  DSerial.print(", ");
  DSerial.println(comp_y); 
// done outside
#else
// falls niemand Werte liefert
  comp_x=1000;  // dummy
  comp_y=1000; 
#endif

#ifdef USE_KOMPASS 
  Wire.beginTransmission(COMP_I2ADDR); // Pollin HDMM01 
  Wire.write(byte(0));   // Adressfeld ist hier nicht wichtig 
  Wire.write(byte(TakeMeasure)); 
  Wire.endTransmission(); 
  delay(20); 
  //delay(50); 

  Wire.beginTransmission(COMP_I2ADDR); // Pollin HDMM01 
  Wire.write(byte(0));   // Adressfeld ist hier nicht wichtig 
  Wire.write(byte(TakeMeasure)); 
  Wire.endTransmission(); 
  delay(20);
  //delay(50); 

  Wire.beginTransmission(COMP_I2ADDR); 
  n =Wire.write(byte(0x01)); 
  if (n != 1)
  {
    DSerial.print("n wrong ");
    DSerial.print(n);
    return -1;
  }
  n = Wire.endTransmission(false); // hold the I2C-bus
  if (! ((n == 0) || (n ==7)) )
  {
    DSerial.print("n2 wrong ");
    DSerial.print(n);
    return -2;
  }    

  Wire.requestFrom(COMP_I2ADDR, 4);
  n=20;   // Timeout  20*20 = 400ms
  while (Wire.available()<4 and n-- >0)        // Timeout !!!!!!!!!!!!!!!!!!!
  {
    //DSerial.print(Wire.available()); 
    //DSerial.print(" ");
    delay(20);
  }
  if (n<=0)  
  {
    DSerial.print("n3 wrong ");
    DSerial.print(n);    
    return -3;
  }
  DSerial.print(n);    
  
  MsbX  =Wire.read();          // obere  4 Bit X 
  LsbX  =Wire.read();          // untere 8 Bit X 
  MsbY  =Wire.read();          // obere  4 Bit Y 
  LsbY  =Wire.read();          // untere 8 Bit Y 
  Wire.endTransmission();         // stop transmitting 

  comp_x=((MsbX&0x0f)*256)+(LsbX); 
  comp_y=((MsbY&0x0f)*256)+(LsbY); 
#endif
  /*
  DSerial.print(x);
  DSerial.print(", ");
  DSerial.print(y);
  DSerial.print(",  (" + String(confvalues.minX) + "," + String(confvalues.maxX) + "," + String(confvalues.minY) + "," + String(confvalues.maxY) +  ")  -> ");
  */
  x = map(comp_x, confvalues.minX, confvalues.maxX, -180, 180); 
  y = map(comp_y, confvalues.minY, confvalues.maxY, -180, 180); 
  /*DSerial.print(x);
  DSerial.print(", ");
  DSerial.print(y);
  DSerial.print(",  --> ");
  */
  //x = map(x, 1900, 2188, -180, 180); 
  //y = map(y, 1910, 2193, -180, 180); 
  mygrad = atan2(x, y)*180/3.1415927410; 

  mygrad += confvalues.angle2;  
  if (mygrad < 0)    
    mygrad = mygrad +360;
  if (mygrad > 360)    
    mygrad = mygrad -360;
  DSerial.print(mygrad);
  DSerial.println("  Grad"); 
  
  return 0;
}

#ifdef USE_TEMP
//---------------------------------------------------------------
void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempC = sensor.getTemp();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}
#endif

#ifdef SHOW_KOMPASS
// Funktion mit x y radius, farbe (Rosette), winkel; zurück String  
String draw_compass(int16_t x, int16_t y, int16_t radius, String farbe, float winkel)
{
  String message5 = "var x=" + String(x) + "; var y=" + String(y) + "; var radius=" + String(radius) + "\n";
  message5 += "ctx.fillStyle = \"rgb(0,0,0)\";";
  message5 += "ctx.fillText(\"" + String(int(winkel*10)/10.0, 1)+ "\",x-38 ,y-180 );\n";  

  String message6="";
  for (int n=0;n<360;n+=20)
  {
    message6+= "ctx.beginPath();"
      "ctx.moveTo(x, y);"
      "ctx.arc(x, y, 120, ";
    message6+= String(n/180.0) + "* 3.1415, " + String((n+10)/180.0);
    message6+=   " * 3.1415, false); ctx.fillStyle =";
    message6+=  farbe + ";" ;
    message6+= "ctx.fill();\n";
  }

  message6+= "    ctx.beginPath();"
      "ctx.arc(x, y, radius, 0, 2 * 3.1415, false); "
      "ctx.lineWidth = 2;"
      "ctx.strokeStyle = '#003300';"
      "ctx.stroke();";

  message6 += "ctx.fillStyle = 'black';";   // 238, 250,
  message6 += "ctx.fillText('S',x-9,        y+radius+35);\n";
  message6 += "ctx.fillText('N',x-9,        y-radius-15);\n";
  message6 += "ctx.fillText('W',x-radius-40,y+10);\n";
  message6 += "ctx.fillText('O',x+radius+12, y+10);\n";
  
// Kreuz
  String message8  =
      "ctx.beginPath();"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x,y-radius-10);"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x,y+radius+10);"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x+radius+10,y);"
      "ctx.moveTo(x, y);"
      "ctx.lineTo(x-radius-10,y);"
      "ctx.lineWidth = 1;"
      "ctx.strokeStyle = '#003300';"
      "ctx.stroke();"
      ;

  String bild;
  int farbe_ = BLAU;
  draw_pic(bild, pfeil, sizeof(pfeil)/sizeof(xy), x, y,  -winkel, 1);  

  return (message5 + message6 + message8 + farben[farbe_] + bild);
}
#endif

unsigned long f_rec=-1, ivalue=-1;  // -1 geht nicht !!!!!!

// alles in char besser ?
unsigned long get_value(String &p, int &pos, int len)
{
  unsigned long value=0;            
  char c;

  if (p.length()-pos < len)  len=p.length()-pos;
  len += pos;   // von Länge zu Index
  for (int m=pos;m<len;m++)
  {
    c=p.charAt(m);
    if (c>='0' and c<='9')
    {
      value= value*10 + c-'0';
      pos++;
    }
    else
      break;
  }
  return value;
}

#ifdef USE_DISPLAY

// Funktionen für TFT Anzeige

void fillRect_(int16_t x, int16_t y, int16_t w, int16_t h,
          uint16_t color) 
{
  for (int16_t i=x; i<x+w; i++) {
    tft_.drawLine(i, y, i, y+h, color);
  }
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

void show_display(void)
{
  char buf[10];
  tft_.setTextSize(2);            
  
  get_height(buf, int(z1), MAX_SHOW,true);
  tft_.setCursor(0,0);
  tft_.print(buf);
  
  if (do_calib)
    strcpy(buf,"cal");
  else if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK ) // test for OK
    strcpy(buf,"OK ");
  else
    strcpy(buf,"   ");
  tft_.setTextColor(ST77XX_BLUE,ST77XX_BLACK); 
  tft_.setCursor(TEXTMITTEOBENX,0);
  tft_.print(buf);
  
  get_height(buf, int(z4), MAX_SHOW,false);
  tft_.setTextColor(ST77XX_WHITE,ST77XX_BLACK);           
  tft_.setCursor(TEXTRECHTSX,0);
  tft_.print(buf);
  
  get_height(buf, int(z2), MAX_SHOW,true);
  tft_.setCursor(0,TEXTUNTENY);
  tft_.print(buf);
  
  get_height(buf, int(z3), MAX_SHOW,false);      
  tft_.setCursor(TEXTRECHTSX,TEXTUNTENY);
  tft_.print(buf);
          
  // void draw_value_v3(int xpos,int ypos, float value, float max_value, int max_height, int breite, strich)
  draw_value_v3_(00 ,VALUEOBENY , z1, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  draw_value_v3_(00 ,VALUEUNTENY, z2, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  draw_value_v3_(VALUERECHTSX,VALUEOBENY , z4, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  draw_value_v3_(VALUERECHTSX,VALUEUNTENY, z3, MAX_VALUE, HEIGHT_BALKEN, WIDTH_BALKEN, keilhoehe);
  
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
    
  draw_Bitmap_faktor(BITMAPOBENX, BITMAPOBENY, p_map, BITMAPOBENW, BITMAPOBENH, 2,2, false, color);          
   
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
    
  bool mirror_=(int(fr_corr) >0);  // int gegen Flackern

  if (!mirror_)
    draw_Bitmap_faktor(BITMAPUNTENX, BITMAPUNTENY, p_map, BITMAPUNTENW, BITMAPUNTENH, 2,2, mirror_, color);          
  else
    draw_Bitmap_faktor(BITMAPUNTENX+22, BITMAPUNTENY, p_map, BITMAPUNTENW, BITMAPUNTENH, 2,2, mirror_, color);          
  
  tft_.setCursor(ANGLEX,ANGLEOBENY);
  get_angle(buf, fp_corr);
  tft_.print(buf);
  
  tft_.setCursor(ANGLEX,ANGLEUNTENY);
  get_angle(buf, fr_corr);
  tft_.print(buf);           
}

#endif

// set up html code for values
void make_value_text(String &text)
{
  text ="<br>"  "<br>"
                  "clients: " + String(nb) + "<br>";
#ifdef SHOW_KOMPASS                  
  text +=         "Grad: " + String(mygrad,0) + "<br>";
 if (do_calib)                  
 {
   text +=  "C: " + String(CmaxX) + ", " + String(CminX) + ", " + String(CmaxY) + ", " + String(CminY) + "<br>"
                    "C_: " + String(comp_x) + ", " + String(comp_y) +"<br>";
   return;
 }
#endif

#ifdef USE_TEMP                  
  text +=         "Temp: " + String(tempC,1) + "C<br>"
                  "Humid:" + String(humidity,1) + "%<br>";
#endif                             
 text +=          "Volt:"  + String(sensorValue/175.5, 2) + "V<br>";                  
}

// set up JS code for painting the canvas
void paint_main(String &message)
{
  DSerial.print(" paint_main ");
  
    message =R"=====(       
    function paintmain()
    {
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    ctx.font = "30px Arial";
    ctx.fillStyle = "rgb(0,0,0)";
    ctx.clearRect(0, 0, canvas.width, canvas.height);    
  )====="; 

// ab hier flexibler Part der Canvas Befehle
  
// wie variabel soll man es machen ? Änderungen hier bedingen auch Änderungen bei den anderen Werten
// alles mit einem Faktor versehen faktorW faktorH ?

  message += "ctx.fillText(\"" + String(int(z1))+ "cm\",80 ,50 );\n";
  message +=       "ctx.fillText(\"" + String(int(z4))+ "cm\",540,50 );\n";
  message +=       "ctx.fillText(\"" + String(int(z2))+ "cm\",80 ,370);\n";
  message +=       "ctx.fillText(\"" + String(int(z3))+ "cm\",540,370);\n";
  
  if ( z1<= HEIGHT_OK and z2<= HEIGHT_OK and z3<= HEIGHT_OK and z4<= HEIGHT_OK ) // test for OK
    message += "ctx.fillStyle = \"rgb(0,200,0)\";\nctx.fillText(\"OK\",335,30);\n";            

  String s1;
  // Balken zeichnen
  draw_value_v3(30 ,18 , z1, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;
  draw_value_v3(30 ,220 , z2, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;
  draw_value_v3(640 ,18 , z4, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;
  draw_value_v3(640 ,220 , z3, MAX_VALUE, HEIGHT_BALKEN_H, WIDTH_BALKEN_H, keilhoehe, s1);
  message += s1;

  // Winkel vorher begradigen oder gleich beim Ausrechnen
  
  message += "ctx.fillStyle = \"rgb(0,0,0)\";";
  message += "ctx.fillText(\"Pitch:" + String(int(fp_corr*10)/10.0, 1)+ "\",200 ,140 );\n";  
  message += "ctx.fillText(\"Roll:" + String(int( (fr_corr <-180 ? -1*(360+fr_corr) :-fr_corr) *10)/10.0, 1)+ "\",405 ,140 );\n";  
  //if (fr_corr <-180)
    //DSerial.print(-1*(360+fr_corr));

  String bild;
  int farbe = GRUEN;
  if      (abs(fp_corr) >=WINKEL_ROT)  farbe=ROT;
  else if (abs(fp_corr) >=WINKEL_GELB) farbe=GELB;
  draw_pic(bild, seite, sizeof(seite)/sizeof(xy), 238, 250,  (float)int(-fp_corr*5), 1.3);
  message += farben[farbe] + bild ;

  farbe = GRUEN;
  if      (abs(fr_corr) >=WINKEL_ROT)  farbe=ROT;
  else if (abs(fr_corr) >=WINKEL_GELB) farbe=GELB;
  draw_pic(bild, back, sizeof(back)/sizeof(xy), 455, 243,  (float)int(-fr_corr*5), 1.3);
  message += farben[farbe] + bild;  

  #ifdef USE_TEMP        
  //if (modeT == MODE_WLAN)
    getWeather(); // für WLAN Zugriff eigens aufrufen, da nicht in der Meßschleife
  #endif

  String values;
  make_value_text(values);
  message += "document.getElementById(\"value_text\").innerHTML = \"" + values + "\";\n "; 

  message += reloadstring;  // Interval umstellen

  if (reloadstring!="")
  {
    if (modeT == MODE_FAST)
    {
      message += "but=document.getElementById(\"Fast\");\n but.value = \"Slow\";\n ";       
    }
    else
    {
      message += "but=document.getElementById(\"Fast\");\n but.value= \"Fast\";\n "; 
    } 
  }
  reloadstring="";
  message += "}\n";  
}

#ifdef SHOW_KOMPASS
// set up JS code for painting the canvas for compass
void paint_compass(String &message)
{
    message =R"=====(       
    function paintcompass()
    {
    var canvas = document.getElementById("myCanvas");
    var ctx = canvas.getContext("2d");
    ctx.font = "30px Arial";
    ctx.fillStyle = "rgb(0,0,0)";
    ctx.clearRect(0, 0, canvas.width, canvas.height);    
  )====="; 

  message += draw_compass(200, 220, 120, "'rgb(120,200,120)'", mygrad);

  String values;
  make_value_text(values);
  message += "document.getElementById(\"value_text\").innerHTML = \"" + values + "\";\n "; 
  message += "}\n";  
}
#endif
